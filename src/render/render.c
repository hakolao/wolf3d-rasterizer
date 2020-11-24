/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/30 18:07:34 by veilo             #+#    #+#             */
/*   Updated: 2020/08/30 18:07:37 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		update_triangle_vertex_zvalues(t_triangle *triangle)
{
	triangle->vtc_zvalue[0] = 1 / (triangle->vtc[0]->pos[2] + L3D_EPSILON);
	triangle->vtc_zvalue[1] = 1 / (triangle->vtc[1]->pos[2] + L3D_EPSILON);
	triangle->vtc_zvalue[2] = 1 / (triangle->vtc[2]->pos[2] + L3D_EPSILON);
}

static void		rasterize(t_sub_framebuffer *sub_buffer,
							t_triangle *triangle, t_render_pass passes)
{
	if ((passes & rpass_zbuffer))
		l3d_triangle_set_zbuffer(sub_buffer, triangle);
	if ((passes & rpass_rasterize))
		l3d_triangle_raster(sub_buffer, triangle);
}

static void		render_triangle(t_wolf3d *app, t_sub_framebuffer *sub_buffer,
									t_triangle *triangle, t_render_pass passes)
{
	t_triangle	clipped_triangles[2];
	t_vertex	vtc[9];
	int32_t		test_clip;

	l3d_set_clipped_triangles(vtc, triangle, clipped_triangles);
	test_clip = l3d_clip_triangle(triangle,
		&app->active_scene->main_camera->viewplanes[0], clipped_triangles);
	if (test_clip == 2)
	{
		screen_intersection(app, &clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[1]);
		update_triangle_vertex_zvalues(&clipped_triangles[0]);
		update_triangle_vertex_zvalues(&clipped_triangles[1]);
		rasterize(sub_buffer, &clipped_triangles[0], passes);
		rasterize(sub_buffer, &clipped_triangles[1], passes);
	}
	else if (test_clip == 1)
	{
		screen_intersection(app, &clipped_triangles[0]);
		update_triangle_vertex_zvalues(&clipped_triangles[0]);
		rasterize(sub_buffer, &clipped_triangles[0], passes);
	}
	else
	{
		screen_intersection(app, triangle);
		update_triangle_vertex_zvalues(triangle);
		rasterize(sub_buffer, triangle, passes);
	}
}

static void		set_render_triangle(t_wolf3d *app,
									t_triangle *r_triangle,
									t_triangle *triangle, t_vertex *vtc)
{
	int32_t		i;

	ft_memcpy(r_triangle, triangle, sizeof(t_triangle));
	i = -1;
	while (++i < 3)
	{
		r_triangle->vtc[i] = &vtc[i];
		ml_vector3_copy(triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_translation, r_triangle->vtc[i]->pos,
			r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_rotation,
			r_triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
	}
	l3d_triangle_update(r_triangle);
}

static void		set_skybox_render_triangle(t_wolf3d *app,
									t_triangle *r_triangle,
									t_triangle *triangle, t_vertex *vtc)
{
	int32_t		i;

	ft_memcpy(r_triangle, triangle, sizeof(t_triangle));
	i = -1;
	while (++i < 3)
	{
		r_triangle->vtc[i] = &vtc[i];
		ml_vector3_copy(triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_rotation,
			r_triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
	}
	l3d_triangle_update(r_triangle);
}

static void		render_skybox(t_rasterize_work *work)
{
	int					i;
	int					j;
	t_triangle			*triangle;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	i = -1;
	while (++i < 6)
	{
		j = -1;
		while (++j < work->app->active_scene->skybox[i]->num_triangles)
		{
			triangle = work->app->active_scene->skybox[i]->triangles + j;
			set_skybox_render_triangle(work->app, &r_triangle, triangle, vtc);
			if (is_rendered(work->app, &r_triangle))
				render_triangle(work->app, work->sub_buffer, &r_triangle,
					rpass_rasterize);
		}
	}
}

static void		rasterize_work(void *params)
{
	t_rasterize_work	*work;
	int					i;
	int					j;
	t_triangle			*triangle;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	work = params;
	render_skybox(work);
	i = -1;
	while (++i < (int)work->app->active_scene->num_objects)
	{
		j = -1;
		while (++j < work->app->active_scene->objects[i]->num_triangles)
		{
			triangle = work->app->active_scene->objects[i]->triangles + j;
			set_render_triangle(work->app, &r_triangle, triangle, vtc);
			if (is_rendered(work->app, &r_triangle))
				render_triangle(work->app, work->sub_buffer, &r_triangle,
					rpass_rasterize | rpass_zbuffer);
		}
	}
	free(work);
}

static void		add_rasterize_work(t_wolf3d *app,
									t_sub_framebuffer *sub_buffer)
{
	t_rasterize_work 	*work;

	error_check(!(work = malloc(sizeof(*work))),
		"Failed to malloc rasterize work");
	work->sub_buffer = sub_buffer;
	work->app = app;
	thread_pool_add_work(app->thread_pool, rasterize_work, work);
}

static void		clear_work(void *params)
{
	t_rasterize_work 	*work;
	t_sub_framebuffer	*sub_buffer;

	work = params;
	sub_buffer = work->sub_buffer;
	l3d_buffer_uint32_clear(sub_buffer->buffer,
		sub_buffer->width * sub_buffer->height, 0x000000FF);
	l3d_buffer_float_clear(sub_buffer->zbuffer,
		sub_buffer->width * sub_buffer->height, FLT_MAX);
	free(work);
}

static void		add_clear_work(t_wolf3d *app,
								t_sub_framebuffer *sub_buffer)
{
	t_rasterize_work 	*work;

	error_check(!(work = malloc(sizeof(*work))),
		"Failed to malloc rasterize work");
	work->sub_buffer = sub_buffer;
	work->app = app;
	thread_pool_add_work(app->thread_pool, clear_work, work);
}

static void		draw_work(void *params)
{
	t_rasterize_work 	*work;
	t_sub_framebuffer	*sub_buffer;
	t_framebuffer		*framebuffer;

	work = params;
	framebuffer = work->app->window->framebuffer;
	sub_buffer = work->sub_buffer;
	l3d_image_place(
		&(t_surface){.h = framebuffer->height, .w = framebuffer->width,
			.pixels = framebuffer->buffer},
		&(t_surface){.h = sub_buffer->height, .w = sub_buffer->width,
			.pixels = sub_buffer->buffer},
		(int32_t[2]){sub_buffer->x_start, sub_buffer->y_start}, 1.0);
	free(work);
}

static void		add_draw_work(t_wolf3d *app,
								t_sub_framebuffer *sub_buffer)
{
	t_rasterize_work 	*work;

	error_check(!(work = malloc(sizeof(*work))),
		"Failed to malloc rasterize work");
	work->sub_buffer = sub_buffer;
	work->app = app;
	thread_pool_add_work(app->thread_pool, draw_work, work);
}

static void		render_scene_parallel(t_wolf3d *app)
{
	int32_t				i;

	i = -1;
	while (++i < app->window->framebuffer->num_x *
		app->window->framebuffer->num_y)
		add_clear_work(app, app->window->framebuffer->sub_buffers[i]);
	thread_pool_wait(app->thread_pool);
	i = -1;
	while (++i < app->window->framebuffer->num_x *
		app->window->framebuffer->num_y)
		add_rasterize_work(app, app->window->framebuffer->sub_buffers[i]);
	thread_pool_wait(app->thread_pool);
	i = -1;
	while (++i < app->window->framebuffer->num_x *
		app->window->framebuffer->num_y)
		add_draw_work(app, app->window->framebuffer->sub_buffers[i]);
	thread_pool_wait(app->thread_pool);
}

void			wolf3d_render(t_wolf3d *app)
{
	if (app->active_scene->main_camera != NULL)
	{
		render_scene_parallel(app);
	}
	ui_render(app);
}

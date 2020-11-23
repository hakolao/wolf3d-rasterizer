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

static void		rasterize_work(void *params)
{
	t_rasterize_work 	*work;

	work = params;
	l3d_triangle_set_zbuffer(work->sub_buffer, work->triangle);
	l3d_triangle_raster(work->sub_buffer, work->triangle);
	free(work);
}

static void		add_rasterize_work(t_wolf3d *app,
									t_sub_framebuffer *sub_buffer,
									t_triangle *triangle)
{
	t_rasterize_work 	*work;

	error_check(!(work = malloc(sizeof(*work))),
		"Failed to malloc rasterize work");
	work->sub_buffer = sub_buffer;
	work->triangle = triangle;
	thread_pool_add_work(app->thread_pool, rasterize_work, work);
}

static void		parallel_rasterize(t_wolf3d *app, t_triangle *triangle)
{
	int32_t				i;

	i = -1;
	while (++i < app->window->framebuffer->num_x *
		app->window->framebuffer->num_y)
	{
		add_rasterize_work(app,
			app->window->framebuffer->sub_buffers[i], triangle);
		break ;
	}
}

static void		render_triangle(t_wolf3d *app, t_triangle *triangle)
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
		parallel_rasterize(app, &clipped_triangles[0]);
		parallel_rasterize(app, &clipped_triangles[1]);
	}
	else if (test_clip ==1)
	{
		screen_intersection(app, &clipped_triangles[0]);
		update_triangle_vertex_zvalues(&clipped_triangles[0]);
		parallel_rasterize(app, &clipped_triangles[0]);
	}
	else
	{
		screen_intersection(app, triangle);
		update_triangle_vertex_zvalues(triangle);
		parallel_rasterize(app, triangle);
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

static void		render_scene(t_wolf3d *app)
{
	int				i;
	int				j;
	t_triangle		*triangle;
	t_triangle		r_triangle;
	t_vertex		vtc[3];

	i = -1;
	while (++i < (int)app->active_scene->num_objects)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
		{
			triangle = app->active_scene->objects[i]->triangles + j;
			set_render_triangle(app, &r_triangle, triangle, vtc);
			if (is_rendered(app, &r_triangle))
				render_triangle(app, &r_triangle);
		}
	}
	thread_pool_wait(app->thread_pool);
}

void			wolf3d_render(t_wolf3d *app)
{
	if (app->active_scene->main_camera != NULL)
	{
		render_scene(app);
	}
	ui_render(app);
}

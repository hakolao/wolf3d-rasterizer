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

static void		rasterize_work(void *params)
{
	t_render_work	*work;
	int					i;
	int					j;
	t_triangle			*triangle;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	work = params;
	rasterize_skybox(work);
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

static void		clear_work(void *params)
{
	t_render_work 	*work;
	t_sub_framebuffer	*sub_buffer;

	work = params;
	sub_buffer = work->sub_buffer;
	l3d_buffer_uint32_clear(sub_buffer->buffer,
		sub_buffer->width * sub_buffer->height, 0x000000FF);
	l3d_buffer_float_clear(sub_buffer->zbuffer,
		sub_buffer->width * sub_buffer->height, FLT_MAX);
	free(work);
}

static void		draw_work(void *params)
{
	t_render_work 	*work;
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

static void		render_work_parallel(t_wolf3d *app,
								void render_work(void *params))
{
	int32_t				i;
	t_render_work 		*work;

	i = -1;
	while (++i < app->window->framebuffer->num_x *
		app->window->framebuffer->num_y)
	{
		error_check(!(work = malloc(sizeof(*work))),
			"Failed to malloc rasterize work");
		work->sub_buffer = app->window->framebuffer->sub_buffers[i];
		work->app = app;
		thread_pool_add_work(app->thread_pool, render_work, work);
	}
	thread_pool_wait(app->thread_pool);
}

static void		render_scene_parallel(t_wolf3d *app)
{
	// First clear sub buffers
	render_work_parallel(app, clear_work);
	// Render everything on sub buffers
	render_work_parallel(app, rasterize_work);
	// Place sub buffers onto main buffer
	render_work_parallel(app, draw_work);
}

void			wolf3d_render(t_wolf3d *app)
{
	if (app->active_scene->main_camera != NULL)
	{
		render_scene_parallel(app);
	}
	ui_render(app);
}

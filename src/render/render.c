/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 23:22:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		clear_buffers(t_render_work *work)
{
	t_sub_framebuffer	*sub_buffer;

	sub_buffer = work->sub_buffer;
	l3d_buffer_uint32_clear(sub_buffer->buffer,
		sub_buffer->width * sub_buffer->height, 0x000000FF);
	l3d_buffer_float_clear(sub_buffer->zbuffer,
		sub_buffer->width * sub_buffer->height, FLT_MAX);
}

static void		draw_buffers(t_render_work *work)
{
	t_sub_framebuffer	*sub_buffer;
	t_framebuffer		*framebuffer;

	sub_buffer = work->sub_buffer;
	framebuffer = work->app->window->framebuffer;
	l3d_image_place(
		&(t_surface){.h = framebuffer->height, .w = framebuffer->width,
			.pixels = framebuffer->buffer},
		&(t_surface){.h = sub_buffer->height, .w = sub_buffer->width,
			.pixels = sub_buffer->buffer},
		(int32_t[2]){sub_buffer->x_start, sub_buffer->y_start}, 1.0);
}

static void		render_work(void *params)
{
	t_render_work		*work;

	work = params;
	if (work->app->is_first_render ||
		(!work->app->active_scene->is_paused &&
			(work->app->player.is_rotating || work->app->player.is_moving)))
	{
		clear_buffers(work);
		rasterize_triangles(work);
	}
	draw_buffers(work);
	free(work);
}

static void		render_work_parallel(t_wolf3d *app)
{
	int32_t				i;
	t_render_work 		*work;
	t_tri_vec			*render_triangles;

	update_camera(app);
	render_triangles = prepare_render_triangles(app);
	app->triangles_in_view = render_triangles->size;
	i = -1;
	while (++i < app->window->framebuffer->num_x *
		app->window->framebuffer->num_y)
	{
		error_check(!(work = malloc(sizeof(*work))),
			"Failed to malloc rasterize work");
		work->sub_buffer = app->window->framebuffer->sub_buffers[i];
		work->app = app;
		work->render_triangles = render_triangles;
		thread_pool_add_work(app->thread_pool, render_work, work);
	}
	thread_pool_wait(app->thread_pool);
	destroy_render_triangles(render_triangles);
	app->is_first_render = false;
}

void			wolf3d_render(t_wolf3d *app)
{
	if (app->active_scene->main_camera != NULL)
	{
		render_work_parallel(app);
	}
	ui_render(app);
}

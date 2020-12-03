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

static void		clear_buffers(t_render_work *work)
{
	t_sub_framebuffer	*sub_buffer;

	sub_buffer = work->sub_buffer;
	l3d_buffer_uint32_clear(sub_buffer->buffer,
		sub_buffer->width * sub_buffer->height, 0x000000FF);
	l3d_buffer_float_clear(sub_buffer->zbuffer,
		sub_buffer->width * sub_buffer->height, FLT_MAX);
}

static void		rasterize(t_render_work *work)
{
	rasterize_skybox(work);
	rasterize_objects(work);
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

static void		rasterize_work(void *params)
{
	t_render_work		*work;

	work = params;
	if (!work->app->active_scene->is_paused)
	{
		clear_buffers(work);
		rasterize(work);
	}
	draw_buffers(work);
	free(work);
}

static void		render_work_parallel(t_wolf3d *app)
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
		thread_pool_add_work(app->thread_pool, rasterize_work, work);
	}
	thread_pool_wait(app->thread_pool);
}

void			wolf3d_render(t_wolf3d *app)
{
	if (app->active_scene->main_camera != NULL)
	{
		render_work_parallel(app);
	}
	ui_render(app);
}

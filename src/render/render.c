/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 02:09:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 15:02:08 by ohakola          ###   ########.fr       */
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
	t_surface			frame;
	t_surface			sub_frame;
	int32_t				xy[0];

	sub_buffer = work->sub_buffer;
	framebuffer = work->app->window->framebuffer;
	frame.w = framebuffer->width;
	frame.h = framebuffer->height;
	frame.pixels = framebuffer->buffer;
	sub_frame.w = sub_buffer->width;
	sub_frame.h = sub_buffer->height;
	sub_frame.pixels = sub_buffer->buffer;
	xy[0] = sub_buffer->x_start;
	xy[1] = sub_buffer->y_start;
	l3d_image_place(&frame, &sub_frame, xy, 1.0);
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

/*
** This is the main render function containing the "flow" of the render pipeline
** 1. Update camera (view boxes)
** 2. Prepare render triangles, it basically outputs a vector of triangles
** to be rendered. A small subset filtered out of all scene object triangles.
** Some triangles are clipped if they intersect with near plane. Those clipped
** triangles are separately allocated in the render triangles. Profiling has
** shown that the preparation is rather fast despite allocation. Most work is
** in rasterization.
** 3. Each sub buffer gets a reference to the render triangles and rasterizes
** those triangles. A sub buffer is a portion of pixels in the screen.
** Render work clears sub buffers, rasterizes and lastly draws onto the main
** framebuffer.
** 4. Parallel work is waited to finish and render triangles are destroyed.
*/

static void		render_work_parallel(t_wolf3d *app)
{
	int32_t				i;
	t_render_work		*work;
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

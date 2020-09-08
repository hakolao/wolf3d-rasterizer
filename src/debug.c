/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 13:08:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/08 13:47:29 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			capture_framerate(t_wolf3d *app)
{
	static uint32_t		delta_time_sum;
	static uint32_t		frames_per_sec;

	app->delta_time = SDL_GetTicks() - app->time_since_start;
	delta_time_sum += app->delta_time;
	frames_per_sec++;
	if (delta_time_sum > 1000.0)
	{
		app->debug_info.fps = frames_per_sec;
		app->debug_info.avg_delta_time =
			(float)delta_time_sum / (float)frames_per_sec;
		delta_time_sum = 0;
		frames_per_sec = 0;
	}
}

void			render_debug_grid(t_wolf3d *app)
{
	int32_t 	i;
	int32_t		interval;
	uint32_t	grid_color;
	uint32_t	axes_color;

	i = 0;
	interval = 20;
	grid_color = rgba_to_u32((SDL_Color){50, 50, 50, 255});
	axes_color = rgba_to_u32((SDL_Color){255, 0, 0, 255});
	while (i < app->main_window->height * app->main_window->width)
	{
		if (i % interval == 0)
		{
			app->main_window->framebuffer[i] = grid_color;
		}
		if (i % (app->main_window->width * interval) < app->main_window->width)
		{
			app->main_window->framebuffer[i] = grid_color;
		}
		if (i / app->main_window->width == app->main_window->height / 2)
			app->main_window->framebuffer[i] = axes_color;
		if (i % app->main_window->width == app->main_window->width / 2)
			app->main_window->framebuffer[i] = axes_color;
	i++;
	}
}

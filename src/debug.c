/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 13:08:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/05 15:11:36 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

uint64_t			capture_framerate(uint64_t delta_time)
{
	static uint64_t		delta_time_sum;
	static uint64_t		frames_per_sec;
	static uint64_t		prev_fps;

	delta_time_sum += delta_time;
	frames_per_sec++;
	if (delta_time_sum > 1000.0)
	{
		prev_fps = frames_per_sec;
		delta_time_sum = 0;
		frames_per_sec = 0;
		return (frames_per_sec);
	}
	return (prev_fps);
}

void			render_debug_grid(t_wolf3d *app)
{
	int32_t 	i;
	int32_t		interval;
	uint32_t	grid_color;
	uint32_t	axes_color;

	i = 0;
	interval = 20;
	grid_color = l3d_rgba_to_u32((uint32_t[4]){50, 50, 50, 255});
	axes_color = l3d_rgba_to_u32((uint32_t[4]){255, 0, 0, 255});
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

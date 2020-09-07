/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 13:08:46 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/07 14:24:33 by ohakola          ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/10/13 18:16:10 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			cap_framerate(t_wolf3d *app)
{
	float		time_per_frame;

	time_per_frame = 1000.0 / (float)FPS;
	if ((uint32_t)app->info.delta_time < time_per_frame)
		SDL_Delay((uint32_t)((uint32_t)time_per_frame - app->info.delta_time));
}

float			sin_time(float min, float max, float speed)
{
	float		amplitude;
	float		zero_offset;
	float		tmp;

	if (min > max)
	{
		tmp = min;
		min = max;
		max = tmp;
	}
	amplitude = (max - min) / 2.0;
	zero_offset = min + amplitude;
	return (amplitude *
		sin((SDL_GetTicks() / 1000.0) * speed ) + zero_offset);
}

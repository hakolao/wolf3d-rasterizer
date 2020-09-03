/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/09/03 13:25:13 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

uint32_t		g_time_since_start;

void			cap_framerate()
{
	float		delta_time;
	float		time_per_frame;

	delta_time = SDL_GetTicks() - g_time_since_start;
	time_per_frame = 1000.0 / (float)FPS;
	if (delta_time < time_per_frame)
		SDL_Delay((uint32_t)(time_per_frame - delta_time));
}

float			sin_time(float min, float max, float speed)
{
	float	amplitude;
	float	zero_offset;
	float	tmp;

	if (min > max)
	{
		tmp = min;
		min = max;
		max = tmp;
	}
	amplitude = (max - min) / 2.0;
	zero_offset = min + amplitude;
	return (amplitude * sin((g_time_since_start / 1000.0) * speed) +
			zero_offset);
}

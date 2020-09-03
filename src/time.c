/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/09/03 13:13:35 by ohakola          ###   ########.fr       */
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

double			sin_time_0_1(float speed)
{
	return ((sin((g_time_since_start / 1000.0) * speed) + 1.0) / 2.0);
}

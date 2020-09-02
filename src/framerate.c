/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framerate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/09/02 14:41:22 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	cap_framerate(uint32_t start_time)
{
	uint32_t		delta_time;
	uint32_t		time_per_frame;

	delta_time = SDL_GetTicks() - start_time;
	time_per_frame = 1000 / FPS + 1;
	if (delta_time < time_per_frame)
		SDL_Delay(time_per_frame - delta_time);
}

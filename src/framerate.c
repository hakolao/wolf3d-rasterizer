/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framerate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/09/02 13:52:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void cap_framerate(Uint32 starting_tick)
{
	if ((1000 / FPS) > SDL_GetTicks() - starting_tick)
	{
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - starting_tick));
	}
}
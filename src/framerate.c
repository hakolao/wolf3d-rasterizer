/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framerate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/08/31 14:34:59 by veilo            ###   ########.fr       */
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
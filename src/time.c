/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 14:34:58 by veilo             #+#    #+#             */
/*   Updated: 2020/11/16 13:56:34 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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

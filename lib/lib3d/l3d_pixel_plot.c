/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_pixel_plot.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:27:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/23 16:56:20 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void		l3d_pixel_plot(uint32_t *buffer, uint32_t dimensions_wh[2],
			int32_t xy[2], uint32_t color)
{
	if (xy[0] < 0 || xy[0] >= (int32_t)dimensions_wh[0] ||
		xy[1] < 0 || xy[1] >= (int32_t)dimensions_wh[1])
		return ;
	buffer[xy[1] * dimensions_wh[0] + xy[0]] = color;
}

uint32_t	l3d_pixel_get(uint32_t *buffer, uint32_t dimensions_wh[2],
				int32_t xy[2])
{
	if (xy[0] < 0 || xy[0] >= (int32_t)dimensions_wh[0] ||
		xy[1] < 0 || xy[1] >= (int32_t)dimensions_wh[1])
		return (INT32_MAX);
	return (buffer[xy[1] * dimensions_wh[0] + xy[0]]);
}

float		l3d_pixel_get_float(float *buffer, uint32_t dimensions_wh[2],
								int32_t xy[2])
{
	if (xy[0] < 0 || xy[0] >= (float)dimensions_wh[0] ||
		xy[1] < 0 || xy[1] >= (float)dimensions_wh[1])
		return (FLT_MAX);
	return (buffer[xy[1] * dimensions_wh[0] + xy[0]]);
}

void		l3d_pixel_plot_float(float *buffer, uint32_t dimensions_wh[2],
					int32_t xy[2], float value)
{
	if (xy[0] < 0 || xy[0] >= (float)dimensions_wh[0] ||
		xy[1] < 0 || xy[1] >= (float)dimensions_wh[1])
		return;
	ft_printf("Index: %d, value: %f, dims: %d %d\n", xy[1] * dimensions_wh[0] + xy[0],
		dimensions_wh[0], dimensions_wh[1]);
	buffer[xy[1] * dimensions_wh[0] + xy[0]] = value;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_pixel_plot.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:27:20 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/27 17:19:51 by ohakola          ###   ########.fr       */
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_image_place.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 14:47:33 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 19:05:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void				l3d_framebuffer_image_place(t_surface *frame,
					t_surface *image, int32_t pos_xy[2], float blend_ratio)
{
	uint32_t		frame_pixel;
	uint32_t		layer_pixel;
	int32_t			x;
	int32_t			y;

	y = pos_xy[1] - 1;
	while (++y < pos_xy[1] + (int32_t)image->h)
	{
		x = pos_xy[0] - 1;
		while (++x < pos_xy[0] + (int32_t)image->w)
		{
			layer_pixel = ((uint32_t*)image->pixels)[(y - pos_xy[1]) *
					image->w + (x - pos_xy[0])];
			frame_pixel =
				frame->pixels[y * frame->w + x];
			if (x < (int32_t)frame->w && y < (int32_t)frame->h &&
				x >= 0 && y >= 0)
				frame->pixels[y * (int32_t)frame->w + x] =
					l3d_color_blend_u32(frame_pixel, layer_pixel, blend_ratio);
		}
	}
}

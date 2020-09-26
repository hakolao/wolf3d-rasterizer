/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 14:05:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 17:32:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void				surface_to_framebuffer(t_wolf3d *app, SDL_Surface *surface,
					float blend_ratio, int xy[2])
{
	uint32_t	framebuffer_pixel;
	uint32_t	surface_pixel;
	int			x;
	int			y;

	y = xy[1] - 1;
	while (++y < xy[1] + surface->h)
	{
		x = xy[0] - 1;
		while (++x < xy[0] + surface->w)
		{
			surface_pixel = ((uint32_t*)surface->pixels)[(y - xy[1]) *
					surface->w + (x -  xy[0])];
			framebuffer_pixel =
				app->main_window->framebuffer[y * app->main_window->width + x];
			if (x < app->main_window->width && y < app->main_window->height &&
				x  >= 0 && y >= 0)
				app->main_window->framebuffer[y * app->main_window->width + x] =
					color_blend_u32(framebuffer_pixel, surface_pixel,
					blend_ratio);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 14:05:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/08 14:00:51 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

SDL_Surface			*surface_from_font(t_wolf3d *app, t_text_params params)
{
	SDL_Surface		*text_surface;
	SDL_Surface		*formatted_surface;

	text_surface = TTF_RenderText_Blended_Wrapped(
		app->main_window->font, params.text, params.text_color,
			app->main_window->width);
	error_check(!text_surface, TTF_GetError());
	formatted_surface = SDL_ConvertSurfaceFormat(text_surface,
		PIXEL_FORMAT, 0);
	SDL_FreeSurface(text_surface);
	error_check(!formatted_surface, SDL_GetError());
	return (formatted_surface);
}

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

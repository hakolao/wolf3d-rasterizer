/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 13:51:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

SDL_Surface			*surface_from_font(t_wolf3d *app,
					const char *text, SDL_Color text_color)
{
	SDL_Surface		*text_surface;
	SDL_Surface		*formatted_surface;

	text_surface =
		TTF_RenderText_Solid(app->main_window->font, text, text_color);
	error_check(!text_surface, TTF_GetError());
	formatted_surface = SDL_ConvertSurfaceFormat(text_surface,
		PIXEL_FORMAT, 0);
	SDL_FreeSurface(text_surface);
	error_check(!formatted_surface, SDL_GetError());
	return (formatted_surface);
}

void				render_blinking_text(t_wolf3d *app, const char *text,
					SDL_Color text_color, int xy[2])
{
	uint32_t	pixel;
	uint32_t	font_pixel;
	int			x;
	int			y;
	SDL_Surface	*surface;

	if (app->main_window->font == NULL)
		app->main_window->font = TTF_OpenFont(GAME_FONT, FONT_SIZE);
	error_check(!app->main_window->font, TTF_GetError());
	surface = surface_from_font(app, text, text_color);
	y = xy[1] - 1;
	while (++y < xy[1] + surface->h)
	{
		x = xy[0] - 1;
		while (++x < xy[0] + surface->w)
		{
			font_pixel =
				((uint32_t*)surface->pixels)[(y - xy[1]) *
					surface->w + (x -  xy[0])];
			pixel =
				app->main_window->framebuffer[y * app->main_window->width + x];
			if (x < app->main_window->width && y < app->main_window->height &&
				x  >= 0 && y >= 0 && font_pixel == rgba_to_u32(text_color))
				app->main_window->framebuffer[y * app->main_window->width + x] =
					color_blend_u32(pixel, font_pixel,
						sin_time(app, 0, 1.0, 4.0));
		}
	}
	SDL_FreeSurface(surface);
}

void			render_ui(t_wolf3d *app)
{
	render_blinking_text(app, "Hello there!",
		(SDL_Color){255, 0, 0, 255},
		(int[2]){app->main_window->width / 2, app->main_window->height / 2});
}

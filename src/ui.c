/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 19:25:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

SDL_Surface		*surface_from_font(t_wolf3d *app,
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

static uint32_t	rgba_to_u32(SDL_Color color)
{
	return (((color.r & 255) << 24) | ((color.g & 255) << 16) |
		((color.b & 255) << 8) | (color.a & 255));
}

static SDL_Color	u32_to_rgba(uint32_t color)
{
	SDL_Color	sdl_color;

	sdl_color.r = (color >> 24) & 255;
	sdl_color.g = (color >> 16) & 255;
	sdl_color.b = (color >> 8) & 255;
	sdl_color.a = (color & 255);
	return (sdl_color);
}

static uint32_t	color_blend_u32(uint32_t color1, uint32_t color2, float ratio)
{
	float		i_ratio;
	SDL_Color	sdl_color1;
	SDL_Color	sdl_color2;
	SDL_Color	new_color;

	if ( ratio > 1.f )
		ratio = 1.f;
	else if ( ratio < 0.f )
		ratio = 0.f;
	i_ratio = 1.f - ratio;
	sdl_color1 = u32_to_rgba(color1);
	sdl_color2 = u32_to_rgba(color2);
	new_color.r = (int)((sdl_color1.r * i_ratio) + (sdl_color2.r * ratio));
	new_color.g = (int)((sdl_color1.g * i_ratio) + (sdl_color2.g * ratio));
	new_color.b = (int)((sdl_color1.b * i_ratio) + (sdl_color2.b * ratio));
	new_color.a = (int)((sdl_color1.a * i_ratio) + (sdl_color2.a * ratio));

    return rgba_to_u32(new_color);
}

void			render_text(t_wolf3d *app, const char *text,
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
					color_blend_u32(pixel, font_pixel, 0.1);
		}
	}
	SDL_FreeSurface(surface);
}

void			render_ui(t_wolf3d *app)
{
	render_text(app, "Hello there!",
		(SDL_Color){255, 0, 0, 255},
		(int[2]){app->main_window->width / 2, app->main_window->height / 2});
}
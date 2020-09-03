/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 14:04:30 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 19:05:39 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static SDL_Surface	*text_surface(t_wolf3d *app, const char *text,
					SDL_Color text_color)
{
	SDL_Surface	*surface;


	if (app->main_window->font == NULL)
		app->main_window->font = TTF_OpenFont(GAME_FONT, get_font_size(app));
	error_check(!app->main_window->font, TTF_GetError());
	surface = surface_from_font(app, text, text_color);
	return (surface);
}

uint32_t			get_font_size(t_wolf3d *app)
{
	float		aspect_ratio;

	aspect_ratio = (float)app->main_window->width /
					(float)app->main_window->height;
	return (app->main_window->width / (float)WIDTH * aspect_ratio * FONT_SIZE);
}

void				render_blinking_text(t_wolf3d *app, const char *text,
					SDL_Color text_color, int xy[2])
{


	SDL_Surface	*surface;

	surface = text_surface(app, text, text_color);
	surface_to_framebuffer(app, surface, sin_time(app, 0, 1.0, 4.0), xy);
	SDL_FreeSurface(surface);
}

void				render_text(t_wolf3d *app, const char *text,
					SDL_Color text_color, int xy[2])
{
	SDL_Surface	*surface;

	surface = text_surface(app, text, text_color);
	surface_to_framebuffer(app, surface, 1.0, xy);
	SDL_FreeSurface(surface);
}

void				render_centered_text(t_wolf3d *app, const char *text,
					SDL_Color text_color, int xy[2])
{
	SDL_Surface	*surface;

	surface = text_surface(app, text, text_color);
	surface_to_framebuffer(app, surface, 1.0, (int[2]){xy[0] - surface->w / 2,
		xy[1] - surface->h / 2});
	SDL_FreeSurface(surface);
}

void				render_centered_blinking_text(t_wolf3d *app, const char *text,
					SDL_Color text_color, int xy[2])
{
	SDL_Surface	*surface;

	surface = text_surface(app, text, text_color);
	surface_to_framebuffer(app, surface, sin_time(app, 0, 1.0, 4.0),
		(int[2]){xy[0] - surface->w / 2, xy[1] - surface->h / 2});
	SDL_FreeSurface(surface);
}

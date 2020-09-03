/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 14:04:30 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 16:17:28 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static SDL_Surface	*text_surface(t_wolf3d *app, const char *text,
					SDL_Color text_color)
{
	SDL_Surface	*surface;
	float		aspect_ratio;

	aspect_ratio = (float)app->main_window->width /
					(float)app->main_window->height;
	if (app->main_window->font == NULL)
		app->main_window->font = TTF_OpenFont(GAME_FONT,
			app->main_window->width / (float)WIDTH * aspect_ratio * FONT_SIZE);
	error_check(!app->main_window->font, TTF_GetError());
	surface = surface_from_font(app, text, text_color);
	return (surface);
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

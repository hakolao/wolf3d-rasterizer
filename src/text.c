/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 14:04:30 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/05 15:07:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static SDL_Surface	*text_surface(t_wolf3d *app, t_text_params params)
{
	SDL_Surface		*text_surface;
	SDL_Surface		*formatted_surface;

	text_surface = TTF_RenderText_Blended_Wrapped(params.font, params.text,
		params.text_color, app->main_window->width);
	error_check(!text_surface, TTF_GetError());
	formatted_surface = SDL_ConvertSurfaceFormat(text_surface,
		PIXEL_FORMAT, 0);
	SDL_FreeSurface(text_surface);
	error_check(!formatted_surface, SDL_GetError());
	return (formatted_surface);
}

uint32_t			get_relative_font_size(t_wolf3d *app, uint32_t font_size)
{
	float		aspect_ratio;

	aspect_ratio = (float)app->main_window->width /
					(float)app->main_window->height;
	return (app->main_window->width / (float)WIDTH * aspect_ratio * font_size);
}

/*
** Note: If you want e.g. blinking text, set blend ratio in params to
** sin_time(app, 0, 1.0, 4.0)
*/

void				render_text(t_wolf3d *app, t_text_params params)
{
	SDL_Surface	*surface;

	surface = text_surface(app, params);
	l3d_framebuffer_image_place(&(t_surface){.h = app->main_window->height,
		.w = app->main_window->width,
		.pixels = app->main_window->framebuffer},
		&(t_surface){.h = surface->h,
		.w = surface->w,
		.pixels = surface->pixels}, params.xy, params.blend_ratio);
	SDL_FreeSurface(surface);
}

void				render_centered_text(t_wolf3d *app, t_text_params params)
{
	SDL_Surface	*surface;

	surface = text_surface(app, params);
	l3d_framebuffer_image_place(&(t_surface){.h = app->main_window->height,
		.w = app->main_window->width,
		.pixels = app->main_window->framebuffer},
		&(t_surface){.h = surface->h,
		.w = surface->w,
		.pixels = surface->pixels},
		(int[2]){params.xy[0] - surface->w / 2, params.xy[1] - surface->h / 2},
		1.0);
	SDL_FreeSurface(surface);
}

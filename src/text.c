/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 14:04:30 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/08 14:26:03 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static SDL_Surface	*text_surface(t_wolf3d *app, t_text_params params)
{
	SDL_Surface	*surface;
	static char	selected_font[128];

	if (!ft_strequ(selected_font, params.font))
	{
		if (app->main_window->font == NULL)
		app->main_window->font = TTF_OpenFont(params.font,
			get_relative_font_size(app, params.font_size));
		else
		{
			TTF_CloseFont(app->main_window->font);
			app->main_window->font = TTF_OpenFont(params.font,
				get_relative_font_size(app, params.font_size));
		}
		error_check(!app->main_window->font, TTF_GetError());
		ft_memcpy(selected_font, params.font, ft_strlen(params.font));
	}
	surface = surface_from_font(app, params);
	return (surface);
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
	surface_to_framebuffer(app, surface, params.blend_ratio, params.xy);
	SDL_FreeSurface(surface);
}

void				render_centered_text(t_wolf3d *app, t_text_params params)
{
	SDL_Surface	*surface;

	surface = text_surface(app, params);
	surface_to_framebuffer(app, surface, 1.0,
		(int[2]){params.xy[0] - surface->w / 2, params.xy[1] - surface->h / 2});
	SDL_FreeSurface(surface);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:19:50 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:32:04 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int		resize_callback(void *data, SDL_Event *event)
{
	t_window_info	*window_info;
	t_wolf3d 		*app;

	if (event->type == SDL_WINDOWEVENT &&
		(event->window.event == SDL_WINDOWEVENT_RESIZED ||
		 event->window.event == SDL_WINDOWEVENT_MINIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_MAXIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_SHOWN ||
		 event->window.event == SDL_WINDOWEVENT_HIDDEN))
		{
		window_info = (t_window_info*)data;
		if (event->window.windowID == window_info->window_id)
		{
			app = (t_wolf3d*)(window_info->parent);
			app->main_window->resized = true;
			if (event->window.event == SDL_WINDOWEVENT_HIDDEN)
				window_info->is_hidden = true;
			else if (event->window.event == SDL_WINDOWEVENT_SHOWN)
				window_info->is_hidden = false;
		}
	}
	return 0;
}

void			main_window_init(t_wolf3d *app)
{
	app->main_window = (t_window*)malloc(sizeof(*(app->main_window)));
	app->main_window->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	error_check(app->main_window->window == NULL, SDL_GetError());
	app->main_window->renderer = SDL_CreateRenderer(app->main_window->window,
													-1,
													SDL_RENDERER_SOFTWARE);
	error_check(app->main_window->renderer == NULL, SDL_GetError());
	app->main_window->frame = SDL_CreateTexture(app->main_window->renderer,
												SDL_PIXELFORMAT_RGBA8888,
												SDL_TEXTUREACCESS_STREAMING,
												WIDTH, HEIGHT);
	error_check(app->main_window->frame == NULL, SDL_GetError());
	app->main_window->window_info.window_id =
		SDL_GetWindowID(app->main_window->window);
	app->main_window->window_info.parent = app;
	app->main_window->window_info.is_hidden = false;
	SDL_AddEventWatch(resize_callback, &app->main_window->window_info);
}
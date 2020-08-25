/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:19:50 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 15:25:08 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int		resize_callback(void *data, SDL_Event *event)
{
	t_window	*window;
	t_wolf3d 	*app;

	if (event->type == SDL_WINDOWEVENT &&
		(event->window.event == SDL_WINDOWEVENT_RESIZED ||
		 event->window.event == SDL_WINDOWEVENT_MINIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_MAXIMIZED ||
		 event->window.event == SDL_WINDOWEVENT_SHOWN ||
		 event->window.event == SDL_WINDOWEVENT_HIDDEN))
		{
		window = (t_window*)data;
		if (event->window.windowID == window->window_id)
		{
			app = (t_wolf3d*)(window->parent);
			app->main_window->resized = true;
			SDL_GetWindowSize(window->window, &app->main_window->width,
				&app->main_window->height);
			if (event->window.event == SDL_WINDOWEVENT_HIDDEN)
				window->is_hidden = true;
			else if (event->window.event == SDL_WINDOWEVENT_SHOWN)
				window->is_hidden = false;
		}
	}
	return 0;
}

void			main_window_init(t_wolf3d *app)
{
	error_check((app->main_window =
		(t_window*)malloc(sizeof(*(app->main_window)))) == NULL,
		"Window malloc failed");
	app->main_window->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	app->main_window->width = WIDTH;
	app->main_window->height = HEIGHT;
	error_check(app->main_window->window == NULL, SDL_GetError());
	app->main_window->renderer =
		SDL_CreateRenderer(app->main_window->window, -1, SDL_RENDERER_SOFTWARE);
	error_check(app->main_window->renderer == NULL, SDL_GetError());
	app->main_window->frame = SDL_CreateTexture(app->main_window->renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	error_check(app->main_window->frame == NULL, SDL_GetError());
	app->main_window->window_id = SDL_GetWindowID(app->main_window->window);
	app->main_window->parent = app;
	app->main_window->is_hidden = false;
	SDL_AddEventWatch(resize_callback, app->main_window);
}
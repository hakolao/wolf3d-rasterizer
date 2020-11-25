/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 13:00:27 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/25 13:00:52 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		handle_menu_events(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->scene_id == scene_id_main_menu)
		main_menu_event_handle(app, event);
	else if (app->active_scene->scene_id == scene_id_main_menu_settings)
		main_menu_settings_event_handle(app, event);
	else if (app->active_scene->scene_id == scene_id_main_game &&
		app->active_scene->is_menu_on)
		main_game_menu_event_handle(app, event);
}

void			handle_events(t_wolf3d *app)
{
	SDL_Event	event;

	mouse_state_set(app);
	mouse_state_handle(app);
	keyboard_state_set(app);
	keyboard_state_handle(app);
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
			event.key.keysym.sym == SDLK_ESCAPE))
		{
			if (app->active_scene->scene_id == scene_id_main_game)
				app->active_scene->is_menu_on = !app->active_scene->is_menu_on;
			else
				app->is_running = false;
		}
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_g)
			app->is_debug = !app->is_debug;
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_f)
		{
			app->window->is_fullscreen = !app->window->is_fullscreen;
			if (app->window->is_fullscreen)
				SDL_SetWindowFullscreen(app->window->window, SDL_WINDOW_FULLSCREEN);
			else
				SDL_SetWindowFullscreen(app->window->window, 0);
		}
		handle_menu_events(app, event);
		if (event.type == SDL_MOUSEMOTION)
			mouse_motion_handle(app, event);
	}
}
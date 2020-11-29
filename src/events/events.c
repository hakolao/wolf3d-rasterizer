/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 13:00:27 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/29 19:59:16 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			minimap_size_update(t_wolf3d *app)
{
	if (app->is_minimap_largened)
	{
		ml_vector2_copy((t_vec2){app->window->width / 2 -
			app->active_scene->map->render_size / 2,
				app->window->height / 2 -
				app->active_scene->map->render_size / 2},
			app->active_scene->map->render_pos);
	}
	else
	{
		ml_vector2_copy((t_vec2){app->window->width -
			app->window->height * 0.3 - 10, 10},
			app->active_scene->map->render_pos);
	}
}

static void		main_game_input_events_handle(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->is_paused)
	{
		app->is_minimap_largened = false;
		minimap_size_update(app);
		main_game_menu_event_handle(app, event);
	}
	else
	{
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_m)
		{
			app->is_minimap_largened = !app->is_minimap_largened;
			minimap_size_update(app);
		}
		mouse_events_handle(app, event);
	}
}

/*
** Handle events that are SDL input events (system events)
*/

static void		game_input_events_handle(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->scene_id == scene_id_main_menu)
		main_menu_event_handle(app, event);
	else if (app->active_scene->scene_id == scene_id_main_menu_settings)
		main_menu_settings_event_handle(app, event);
	else if (app->active_scene->scene_id == scene_id_main_game)
		main_game_input_events_handle(app, event);
}

/*
** Handle events and behavior that's dependent on keyboard or mouse state
** (what buttons are down, what buttons are up)
*/

static void		game_input_state_handle(t_wolf3d *app)
{
	mouse_state_set(app);
	mouse_state_handle(app);
	keyboard_state_set(app);
	keyboard_state_handle(app);
}

/*
** Handle events that aren't related to menus or game, like exiting or esc
** or setting to full screen, or disabling debug info
*/

static void		general_input_events_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_QUIT)
		app->is_running = false;
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
	{
		if (app->active_scene->scene_id == scene_id_main_game)
			app->active_scene->is_paused = !app->active_scene->is_paused;
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
}

/*
** Main API for event handling in wolf3d
*/

void			events_handle(t_wolf3d *app)
{
	SDL_Event	event;

	if (!app->active_scene->is_paused)
		game_input_state_handle(app);
	while (SDL_PollEvent(&event))
	{
		general_input_events_handle(app, event);
		game_input_events_handle(app, event);
	}
}

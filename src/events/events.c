/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/09 15:42:23 by ohakola          ###   ########.fr       */
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
	}
}

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
** Main API for event handling in wolf3d
** 1. Poll state and handle keyboard & mouse state (in-game actions mostly
** where there should not be any lag)
** 2. General input events, e.g. debug mode, set normal map shading, set
** full screen etc.
** 3. Game input events: menu options, pausing, etc.
*/

void			events_handle(t_wolf3d *app)
{
	SDL_Event	event;

	if (!app->active_scene->is_paused)
	{
		SDL_PumpEvents();
		mouse_state_handle(app);
		keyboard_state_handle(app);
	}
	while (SDL_PollEvent(&event))
	{
		general_input_events_handle(app, event);
		game_input_events_handle(app, event);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 13:00:27 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/26 15:53:54 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		update_minimap_size(t_wolf3d *app)
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

static void		handle_game_events(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->scene_id == scene_id_main_menu)
		main_menu_event_handle(app, event);
	else if (app->active_scene->scene_id == scene_id_main_menu_settings)
		main_menu_settings_event_handle(app, event);
	else if (app->active_scene->scene_id == scene_id_main_game)
	{
		if (app->active_scene->temp_objects)
			l3d_temp_objects_destroy_if_expired(
				&app->active_scene->temp_objects,
				SDL_GetTicks(), TEMP_OBJECT_EXPIRE_SEC * 1000);
		if (app->active_scene->is_paused)
		{
			app->is_minimap_largened = false;
			update_minimap_size(app);
			main_game_menu_event_handle(app, event);
		}
		else
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_m)
			{
				app->is_minimap_largened = !app->is_minimap_largened;
				update_minimap_size(app);
			}
			if (event.type == SDL_MOUSEMOTION)
				mouse_motion_handle(app, event);
			if (event.type == SDL_MOUSEBUTTONDOWN &&
				event.button.button == SDL_BUTTON_LMASK)
				shooting_handle(app);
		}
	}
}


static void		handle_game_input_state(t_wolf3d *app)
{
	mouse_state_set(app);
	keyboard_state_set(app);
	movement_handle(app);
}

void			handle_events(t_wolf3d *app)
{
	SDL_Event	event;

	if (!app->active_scene->is_paused)
		handle_game_input_state(app);
	while (SDL_PollEvent(&event))
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
		handle_game_events(app, event);
	}
}

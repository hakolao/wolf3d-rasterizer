/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:36:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/25 12:31:07 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		keyboard_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	app->keyboard.state = SDL_GetKeyboardState(NULL);
}

void		keyboard_state_handle(t_wolf3d *app)
{
	if (app->active_scene->scene_id == scene_id_main_game)
	{
		if (app->keyboard.state[SDL_SCANCODE_W])
			move_player(app, move_forward);
		if (app->keyboard.state[SDL_SCANCODE_A])
			move_player(app, move_strafe_left);
		if (app->keyboard.state[SDL_SCANCODE_S])
			move_player(app, move_backward);
		if (app->keyboard.state[SDL_SCANCODE_D])
			move_player(app, move_strafe_right);
		if (app->keyboard.state[SDL_SCANCODE_LSHIFT])
			app->player.is_running = true;
		else
			app->player.is_running = false;
	}
}

static void	menu_options_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.key.keysym.sym == SDLK_UP)
		app->active_scene->selected_option--;
	else if (event.key.keysym.sym == SDLK_DOWN)
		app->active_scene->selected_option++;
	if (app->active_scene->selected_option >=
		app->active_scene->menu_option_count)
		app->active_scene->selected_option = 0;
	if (app->active_scene->selected_option < 0)
		app->active_scene->selected_option =
			app->active_scene->menu_option_count - 1;
}

void 		main_menu_event_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_KEYUP)
	{
		menu_options_handle(app, event);
		if (event.key.keysym.sym == SDLK_RETURN)
		{
			if (app->active_scene->selected_option == 0)
			{
				app->next_scene_id = scene_id_main_game;
				select_next_scene(app);
			}
			else if (app->active_scene->selected_option == 2)
			{
				app->next_scene_id = scene_id_main_menu_settings;
				select_next_scene(app);
			}
			else if (app->active_scene->selected_option == 3)
				app->is_running = false;
		}
	}
}

void 		main_menu_settings_event_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_KEYUP)
	{
		menu_options_handle(app, event);
		if (event.key.keysym.sym == SDLK_RETURN)
		{
			if (app->active_scene->selected_option == 0)
				window_resize(app->window, 1280, 720);
			else if (app->active_scene->selected_option == 1)
				window_resize(app->window, 1920, 1080);
			else if (app->active_scene->selected_option == 2)
			{
				app->next_scene_id = scene_id_main_menu;
				select_next_scene(app);
			}
		}
	}
}

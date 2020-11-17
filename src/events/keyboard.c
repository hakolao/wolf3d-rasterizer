/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:36:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/17 17:58:58 by ohakola+vei      ###   ########.fr       */
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
	}
}

void 		main_menu_event_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_KEYUP)
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
		if (event.key.keysym.sym == SDLK_RETURN)
		{
			if (app->active_scene->selected_option == menu_action_start_game)
				set_active_scene(app, scene_id_main_game);
			else if (app->active_scene->selected_option == menu_action_quit_game)
				app->is_running = false;
		}
	}
}

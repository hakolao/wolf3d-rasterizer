/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:36:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/03 15:59:46 by ohakola+vei      ###   ########.fr       */
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
	float	speed;

	if (app->active_scene->scene_id == scene_id_main_game)
	{
		speed = (app->player.is_running ?
			app->player.speed * 1.5 : app->player.speed) *
				app->info.delta_time;
		if (app->keyboard.state[SDL_SCANCODE_W])
			player_move(app, move_forward, speed);
		if (app->keyboard.state[SDL_SCANCODE_A])
			player_move(app, move_strafe_left, speed);
		if (app->keyboard.state[SDL_SCANCODE_S])
			player_move(app, move_backward, speed);
		if (app->keyboard.state[SDL_SCANCODE_D])
			player_move(app, move_strafe_right, speed);
		if (!(app->keyboard.state[SDL_SCANCODE_W]) &&
			!(app->keyboard.state[SDL_SCANCODE_A]) &&
			!(app->keyboard.state[SDL_SCANCODE_S]) &&
			!(app->keyboard.state[SDL_SCANCODE_D]))
			app->player.is_moving = false;
		if (app->keyboard.state[SDL_SCANCODE_LSHIFT])
			app->player.is_running = true;
		else
			app->player.is_running = false;
	}
}

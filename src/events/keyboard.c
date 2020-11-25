/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:36:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/25 13:33:40 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		keyboard_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	app->keyboard.state = SDL_GetKeyboardState(NULL);
}

void		movement_handle(t_wolf3d *app)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/30 01:44:35 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void 			mouse_motion_handle(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->scene_id == scene_id_main_game)
	{
		player_rotate_vertical(app, (float)event.motion.yrel * 0.3);
		player_rotate_horizontal(app, -(float)event.motion.xrel * 0.3);
	}
}

void					mouse_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
}

void					mouse_state_handle(t_wolf3d *app)
{
	if (app->active_scene->scene_id != scene_id_main_game)
		return ;
	if (!app->player.is_shooting && (app->mouse.state & SDL_BUTTON_LMASK))
	{
		app->player.is_shooting = true;
		player_shoot(app, SDL_GetTicks());
	}
	else if (!(app->mouse.state & SDL_BUTTON_LMASK))
	{
		app->player.is_shooting = false;
	}
	if (app->player.is_shooting)
		player_shoot(app, SDL_GetTicks());
}

void					mouse_events_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_MOUSEMOTION)
		mouse_motion_handle(app, event);
}


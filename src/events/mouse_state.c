/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/09 15:26:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void				mouse_motion_handle(t_wolf3d *app,
							int32_t xrel, int32_t yrel)
{
	if (xrel != 0 || yrel != 0)
	{
		app->player.is_rotating = true;
		player_rotate_vertical(app, (float)yrel * 0.3);
		player_rotate_horizontal(app, -(float)xrel * 0.3);
	}
	else
		app->player.is_rotating = false;
}

void					mouse_state_handle(t_wolf3d *app)
{
	int32_t	prev_x;
	int32_t	prev_y;
	int32_t	xrel;
	int32_t	yrel;

	SDL_PumpEvents();
	prev_x = app->mouse.x;
	prev_y = app->mouse.y;
	SDL_GetRelativeMouseState(&xrel, &yrel);
	app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
	if (app->active_scene->scene_id != scene_id_main_game)
		return ;
	mouse_motion_handle(app, xrel, yrel);
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

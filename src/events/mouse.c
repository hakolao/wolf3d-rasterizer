/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/29 15:29:31 by ohakola+vei      ###   ########.fr       */
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

void					mouse_events_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_MOUSEMOTION)
		mouse_motion_handle(app, event);
	if (event.type == SDL_MOUSEBUTTONDOWN &&
		event.button.button == SDL_BUTTON_LMASK)
		player_shoot(app);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/15 14:40:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void					mouse_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	if (SDL_GetMouseFocus() != app->window->window)
		return ;
	app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
}

void					mouse_state_handle(t_wolf3d *app)
{
	(void)app;
}

void 					mouse_motion_handle(t_wolf3d *app, SDL_Event event)
{
	rotate_player_vertical(app, (float)event.motion.yrel * 0.3);
	rotate_player_horizontal(app, -(float)event.motion.xrel * 0.3);
}

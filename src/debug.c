/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:17:51 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 18:25:28 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			wolf3d_debug_info_render(t_wolf3d *app)
{
	char	debug_info[1024];
	char	pos[64];
	char	dir[64];

	ml_vector3_to_str(app->player.pos, pos);
	ml_vector3_to_str(app->player.forward, dir);
	ft_sprintf(debug_info,
		"fps: %u\n"
		"delta time: %u\n"
		"pos: %s"
		"dir: %s",
		app->info.fps,
		app->info.delta_time,
		pos,
		dir);
	window_text_render(app->window, (t_text_params){
		.text = debug_info, .blend_ratio = 1.0,
		.xy = (int[2]){5, 5},
		.text_color = (SDL_Color){255, 255, 255, 0}},
		app->window->debug_font);
}

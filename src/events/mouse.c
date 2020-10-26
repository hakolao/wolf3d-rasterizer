/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/26 16:51:55 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void					mouse_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
}

void					mouse_state_handle(t_wolf3d *app)
{
	t_mat4	rotation_x;
	t_mat4	rotation_y;
	t_vec3	tmp;
	t_vec3	forward;
	t_ray	ray;
	t_hit	hit;
	
	if (app->active_scene->scene_id == scene_id_main_game)
	{
		if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK))
		{
			ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
			ml_matrix4_rotation_x(ml_rad(app->player.rot_y), rotation_y);
			ml_matrix4_mul_vec3(rotation_x, app->player.forward, tmp);
			ml_matrix4_mul_vec3(rotation_y, tmp, forward);
			ml_vector3_mul(forward, -1, forward);
			l3d_ray_set(forward, app->player.pos, &ray);
			if (l3d_kd_tree_ray_hit(app->active_scene->collision_tree->root, &ray, &hit))
			{
				ft_printf("Hits\n");
				ml_vector3_print(hit.hit_point);
			}
		}
	}
}

void 					mouse_motion_handle(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->scene_id == scene_id_main_game)
	{
		rotate_player_vertical(app, (float)event.motion.yrel * 0.3);
		rotate_player_horizontal(app, -(float)event.motion.xrel * 0.3);
	}
}

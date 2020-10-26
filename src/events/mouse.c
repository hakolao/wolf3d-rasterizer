/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/26 19:36:56 by ohakola          ###   ########.fr       */
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
	t_vec3	forward;
	t_ray	ray;
	t_hit	hit;
	
	if (app->active_scene->scene_id == scene_id_main_game)
	{
		if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK))
		{
			ml_vector3_mul(app->player.forward, -1, forward);
			l3d_ray_set(forward, app->active_scene->main_camera->origin, &ray);
			if (l3d_kd_tree_ray_hit(app->active_scene->collision_tree->root, &ray, &hit))
			{
				ft_printf("Hits\n");
				ml_vector3_print(hit.hit_point);
				if (hit.triangle->material->texture)
				{
					t_vec2 	points2d[3];
					float 	uv[2];
					float	barycoords[3];
					ml_vector2_copy(hit.triangle->vtc[0]->pos, points2d[0]);
					ml_vector2_copy(hit.triangle->vtc[1]->pos, points2d[1]);
					ml_vector2_copy(hit.triangle->vtc[2]->pos, points2d[2]);
					l3d_calculate_barycoords(points2d, hit.hit_point, barycoords);
					l3d_interpolate_uv(hit.triangle, barycoords, uv);
					int	index;
					index = (int)(uv[1] * hit.triangle->material->height) *
						hit.triangle->material->width +
						(int)(uv[0] * hit.triangle->material->width);
					if (index >= (int)hit.triangle->material->width *
						(int)hit.triangle->material->height)
						return ;
					hit.triangle->material->texture[index] = 0xFFFFFFFF;
				}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/06 14:07:41 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void					mouse_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
}

static void				interpolate_uv(t_triangle *triangle, float *baryc,
									t_vec2 uv)
{
	uv[0] = (baryc[0] * triangle->uvs[0][0] +
		baryc[1] * triangle->uvs[1][0] +
		baryc[2] * triangle->uvs[2][0]) /
			(baryc[0] +
			baryc[1] +
			baryc[2]);
	uv[1] = 1 - (baryc[0] * triangle->uvs[0][1] +
		baryc[1] * triangle->uvs[1][1] +
		baryc[2] * triangle->uvs[2][1]) /
			(baryc[0] * 1 +
			baryc[1] * 1 +
			baryc[2] * 1);
}

static void				draw_object_hit(t_hit *hit)
{
	t_vec2 	points2d[3];
	float 	uv[2];
	float	barycoords[3];
	int32_t	index;

	ml_vector2_copy(hit->triangle->vtc[0]->pos, points2d[0]);
	ml_vector2_copy(hit->triangle->vtc[1]->pos, points2d[1]);
	ml_vector2_copy(hit->triangle->vtc[2]->pos, points2d[2]);
	l3d_calculate_barycoords(points2d, hit->hit_point, barycoords);
	interpolate_uv(hit->triangle, barycoords, uv);
	index = (int32_t)(uv[1] * hit->triangle->material->height) *
		hit->triangle->material->width +
		(int32_t)(uv[0] * hit->triangle->material->width);
	if (index < 0 || index >= (int32_t)hit->triangle->material->width *
		(int32_t)hit->triangle->material->height)
		return ;
	hit->triangle->material->texture[index] = 0xFFFFFFFF;
}

void					mouse_state_handle(t_wolf3d *app)
{
	t_ray	ray;
	t_hit	hit;

	if (app->active_scene->scene_id == scene_id_main_game)
	{
		if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK))
		{
			l3d_ray_set(app->player.forward, app->player.pos, &ray);
			if (l3d_kd_tree_ray_hit(app->active_scene->bullet_tree->root, &ray, &hit))
			{
				if (hit.triangle->material->texture)
					draw_object_hit(&hit);
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

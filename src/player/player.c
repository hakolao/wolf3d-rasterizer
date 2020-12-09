/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/09 13:52:30 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			player_update_aabb(t_player *player)
{
	player->aabb.xyz_min[0] = player->pos[0] - player->aabb.size[0] / 2.0;
	player->aabb.xyz_min[1] = player->pos[1] - player->aabb.size[1] / 2.0;
	player->aabb.xyz_min[2] = player->pos[2] - player->aabb.size[2] / 2.0;
	player->aabb.xyz_max[0] = player->pos[0] + player->aabb.size[0] / 2.0;
	player->aabb.xyz_max[1] = player->pos[1] + player->aabb.size[1] / 2.0;
	player->aabb.xyz_max[2] = player->pos[2] + player->aabb.size[2] / 2.0;
	ml_vector3_copy(player->pos, player->aabb.center);
}

void			player_init(t_wolf3d *app, t_vec3 pos)
{
	ft_memset(&app->player, 0, sizeof(t_player));
	ft_memcpy(app->player.pos, pos, sizeof(t_vec3));
	ft_memcpy(app->player.forward, &(t_vec3){0, 0, -1}, sizeof(t_vec3));
	ft_memcpy(app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(app->player.sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	app->player.speed = PLAYER_SPEED;
	app->player.rot_speed = PLAYER_ROTATION_SPEED;
	app->player.rot_x = 0;
	app->player.rot_y = 0;
	app->player.player_height = 1.75 * app->unit_size;
	app->player.fire_rate_per_sec = 4.0;
	app->player.aabb.size[0] = app->unit_size / 2.0;
	app->player.aabb.size[1] = app->player.player_height;
	app->player.aabb.size[2] = app->unit_size / 2.0;
	ml_matrix4_id(app->player.rotation);
	ml_matrix4_id(app->player.inv_rotation);
	ml_matrix4_id(app->player.translation);
	ml_matrix4_id(app->player.inv_translation);
	pos_to_grid_pos(app->player.pos, app->player.grid_pos, app->unit_size);
	player_update_aabb(&app->player);
}

void			place_player(t_wolf3d *app, float unit_size,
					int32_t xy_rot[3])
{
	player_init(app, (t_vec3){(float)xy_rot[1] * (2 * unit_size), 0,
		-(float)xy_rot[0] * (2 * unit_size)});
	player_rotate_horizontal(app, xy_rot[2]);
}

/*
** 	Clicking shoots right away. Else, fired according to fire rate
** // ToDo: Add various weapons & fire rates etc.
** // ToDo: Add bullet temp objects to hit surface
*/

void			player_shoot(t_wolf3d *app, uint32_t curr_time)
{
	t_vec3			origin;
	t_vec3			add;
	t_hits			*hits;
	t_hit			*closest_triangle_hit;
	static uint32_t	prev_shot_time;

	if (prev_shot_time != 0 && (float)(curr_time - prev_shot_time) / 1000.0 <
		(1.0 / app->player.fire_rate_per_sec))
		return ;
	prev_shot_time = SDL_GetTicks();
	hits = NULL;
	ml_vector3_mul(app->player.forward, NEAR_CLIP_DIST, add);
	ml_vector3_add(app->player.pos, add, origin);
	if (l3d_kd_tree_ray_hits(app->active_scene->triangle_tree, origin,
		app->player.forward, &hits))
	{
		l3d_get_closest_hit(hits, &closest_triangle_hit);
		if (closest_triangle_hit != NULL)
		{
		}
		l3d_delete_hits(&hits);
	}
}

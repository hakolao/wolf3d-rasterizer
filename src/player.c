/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 14:13:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		apply_movement(t_player *player, t_vec3 new_pos)
{

	ft_memcpy(&player->pos, new_pos, sizeof(t_vec3));
	update_camera_view(player);
}


void			init_player(t_wolf3d *app)
{
	ft_memcpy(&app->player.pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&app->player.forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	app->player.speed = 0.5f;
	init_camera(&app->player);
}

void			rotate_player(t_player *player, float angle, t_vec3 axes)
{
	t_vec3	new_direction;
	t_mat4	rotation;

	ml_vector3_mul(axes, ml_rad(player->rot_speed * angle), axes);
	ml_matrix4_rotation(axes[0], axes[1], axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, player->forward, new_direction);
	ft_memcpy(&player->forward, new_direction, sizeof(t_vec3));
	update_camera_view(player);
}

void			move_player(t_player *player, t_move dir)
{
	t_vec3	add;
	t_vec3	new_pos;
	t_vec3	sideways;
	
	if (dir == forward)
	{
		ml_vector3_mul(player->forward, player->speed, add);
		ml_vector3_add(player->pos, add, new_pos);
	}
	else if (dir == backward)
	{
		ml_vector3_mul(player->forward, player->speed, add);
		ml_vector3_sub(player->pos, add, new_pos);
	}
	else if (dir == strafe_left)
	{
		ml_vector3_cross(player->forward, player->up, sideways);
		ml_vector3_add(player->pos, add, new_pos);
	}
	else if (dir == strafe_right)
	{
		ml_vector3_cross(player->forward, player->up, sideways);
		ml_vector3_sub(player->pos, add, new_pos);
	}
	apply_movement(player, new_pos);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/07 16:45:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		apply_movement(t_wolf3d *app, t_vec3 new_pos)
{
	t_player *player;

	player = &app->player;
	ft_memcpy(&player->pos, new_pos, sizeof(t_vec3));
	// ToDo: Update world view based on player movement
}


void			init_player(t_wolf3d *app)
{
	ft_memcpy(&app->player.pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&app->player.forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	app->player.speed = 0.5f;
}

void			rotate_player(t_player *player, float angle, t_vec3 axes)
{
	t_vec3	new_direction;
	t_mat4	rotation;

	ml_vector3_mul(axes, ml_rad(player->rot_speed * angle), axes);
	ml_matrix4_rotation(axes[0], axes[1], axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, player->forward, new_direction);
	ft_memcpy(&player->forward, new_direction, sizeof(t_vec3));
}

void			move_player(t_wolf3d *app, t_move dir)
{
	t_vec3		add;
	t_vec3		new_pos;
	t_vec3		sideways;
	t_player	*player;

	player = &app->player;
	if (dir == move_forward)
	{
		ml_vector3_mul(player->forward, player->speed, add);
		ml_vector3_add(player->pos, add, new_pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(player->forward, -player->speed, add);
		ml_vector3_add(player->pos, add, new_pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_cross(player->forward, player->up, sideways);
		ml_vector3_mul(sideways, player->speed, sideways);
		ml_vector3_sub(player->pos, sideways, new_pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_cross(player->forward, player->up, sideways);
		ml_vector3_mul(sideways, player->speed, sideways);
		ml_vector3_add(player->pos, sideways, new_pos);
	}
	apply_movement(app, new_pos);
}

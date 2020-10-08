/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/08 21:17:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			init_player(t_wolf3d *app)
{
	ft_memcpy(&app->player.pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&app->player.forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(&app->player.sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	app->player.speed = PLAYER_SPEED;
	app->player.rot_speed = PLAYER_ROTATION_SPEED;
	ml_matrix4_id(app->player.rotation);
	ml_matrix4_id(app->player.inv_rotation);
	ml_matrix4_id(app->player.translation);
}


void			rotate_player(t_wolf3d *app, t_vec3 axes)
{
	t_mat4	rotation;
	t_mat4	new_rotation;

	ml_vector3_mul(axes, ml_rad(app->player.rot_speed * app->delta_time), axes);
	ml_matrix4_rotation(axes[0], axes[1], axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, app->player.forward, app->player.forward);
	ml_matrix4_mul_vec3(rotation, app->player.up, app->player.up);
	ml_matrix4_mul_vec3(rotation, app->player.sideways, app->player.sideways);
	ml_matrix4_mul(app->player.rotation, rotation, new_rotation);
	ft_memcpy(app->player.rotation, new_rotation, sizeof(t_mat4));
	ml_matrix4_inverse(app->player.rotation, app->player.inv_rotation);
}

void			move_player(t_wolf3d *app, t_move dir)
{
	t_vec3		add;
	t_vec3		new_pos;

	if (dir == move_forward)
	{
		ml_vector3_mul(app->player.forward, app->player.speed * app->delta_time, add);
		ml_vector3_add(app->player.pos, add, new_pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(app->player.forward, -app->player.speed * app->delta_time, add);
		ml_vector3_add(app->player.pos, add, new_pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_mul(app->player.sideways, -app->player.speed * app->delta_time, add);
		ml_vector3_sub(app->player.pos, add, new_pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_mul(app->player.sideways, app->player.speed * app->delta_time, add);
		ml_vector3_sub(app->player.pos, add, new_pos);
	}
	ml_vector3_copy(new_pos, app->player.pos);
	ml_matrix4_translation(app->player.pos[0], app->player.pos[1],
		app->player.pos[2], app->player.translation);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:56:32 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			init_player(t_wolf3d *app, t_vec3 pos)
{
	ft_memcpy(app->player.pos, pos, sizeof(t_vec3));
	ft_memcpy(app->player.forward, &(t_vec3){0, 0, -1}, sizeof(t_vec3));
	ft_memcpy(app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(app->player.sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	app->player.speed = PLAYER_SPEED;
	app->player.rot_speed = PLAYER_ROTATION_SPEED;
	app->player.rot_x = 0;
	app->player.rot_y = 0;
	ml_matrix4_id(app->player.rotation);
	ml_matrix4_id(app->player.inv_rotation);
	ml_matrix4_id(app->player.translation);
	ml_matrix4_id(app->player.inv_translation);
}

static void		rotate_player(t_wolf3d *app)
{
	t_mat4	rotation_y;
	t_mat4	rotation_x;
	t_mat4	rotation;

	ml_matrix4_rotation_x(ml_rad(app->player.rot_y), rotation_y);
	ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
	ml_matrix4_mul(rotation_x, rotation_y, rotation);
	ml_matrix4_mul_vec3(rotation, (t_vec3){0, 0, -1}, app->player.forward);
	ml_matrix4_mul_vec3(rotation, (t_vec3){1, 0, 0}, app->player.sideways);
	ml_matrix4_inverse(rotation, app->player.inv_rotation);
}

void			rotate_player_vertical(t_wolf3d *app, float angle)
{
	app->player.rot_y += app->player.rot_speed * angle;
	rotate_player(app);
}

void			rotate_player_horizontal(t_wolf3d *app, float angle)
{
	app->player.rot_x += app->player.rot_speed * angle;
	rotate_player(app);
}

void			move_player(t_wolf3d *app, t_move dir)
{
	t_vec3		add;
	t_vec3		forward;
	t_vec3		sideways;
	t_mat4		rotation_x;

	ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){0, 0, -1}, forward);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){1, 0, 0}, sideways);
	if (dir == move_forward)
	{
		ml_vector3_mul(forward, app->player.speed * app->info.delta_time, add);
		ml_vector3_add(app->player.pos, add, app->player.pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(forward, -app->player.speed * app->info.delta_time, add);
		ml_vector3_add(app->player.pos, add, app->player.pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_mul(sideways, app->player.speed * app->info.delta_time, add);
		ml_vector3_sub(app->player.pos, add, app->player.pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_mul(sideways, -app->player.speed * app->info.delta_time, add);
		ml_vector3_sub(app->player.pos, add, app->player.pos);
	}
	ml_matrix4_translation(app->player.pos[0],
		app->player.pos[1], app->player.pos[2], app->player.translation);
	ml_matrix4_inverse(app->player.translation, app->player.inv_translation);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/03 22:47:36 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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
	ml_matrix4_id(app->player.rotation);
	ml_matrix4_id(app->player.inv_rotation);
	ml_matrix4_id(app->player.translation);
	ml_matrix4_id(app->player.inv_translation);
	pos_to_grid_pos(app->player.pos, app->player.grid_pos, app->unit_size);
}

static void		player_rotate(t_wolf3d *app)
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

void			player_rotate_vertical(t_wolf3d *app, float angle)
{
	app->player.rot_y += app->player.rot_speed * angle;
	if (app->player.rot_y > 90.0)
		app->player.rot_y = 90.0;
	if (app->player.rot_y < -90.0)
		app->player.rot_y = -90.0;
	player_rotate(app);
}

void			player_rotate_horizontal(t_wolf3d *app, float angle)
{
	app->player.rot_x += app->player.rot_speed * angle;
	player_rotate(app);
}

/*
** / 2.0 because models are positioned with unitsize * 2
*/

void			pos_to_grid_pos(t_vec3 pos, t_vec2 grid_pos, float unit_size)
{
	grid_pos[0] = -(pos[2] / unit_size / 2.0) + 0.5;
	grid_pos[1] = (pos[0] / unit_size / 2.0) + 0.5;
}

void			player_move(t_wolf3d *app, t_move dir, float speed)
{
	t_vec3		add;
	t_vec3		forward;
	t_vec3		sideways;
	t_mat4		rotation_x;

	app->player.is_moving = true;
	ft_memset(add, 0, sizeof(t_vec3));
	ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){0, 0, -1}, forward);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){1, 0, 0}, sideways);
	if (dir == move_forward)
		ml_vector3_mul(forward, speed, add);
	else if (dir == move_backward)
		ml_vector3_mul(forward, -speed, add);
	else if (dir == move_strafe_left)
		ml_vector3_mul(sideways, -speed, add);
	else if (dir == move_strafe_right)
		ml_vector3_mul(sideways, speed, add);
	else if (dir == move_up)
		ml_vector3_mul((t_vec3){0, -1, 0}, speed, add);
	else if (dir == move_down)
		ml_vector3_mul((t_vec3){0, 1, 0}, speed, add);
	ml_vector3_add(app->player.pos, add, app->player.pos);
	ml_matrix4_translation(app->player.pos[0],
		app->player.pos[1], app->player.pos[2], app->player.translation);
	ml_matrix4_inverse(app->player.translation, app->player.inv_translation);
	pos_to_grid_pos(app->player.pos, app->player.grid_pos, app->unit_size);
}

/*
** 	Clicking shoots right away. Else, fired according to fire rate
** // ToDo: Add various weapons & fire rates etc.
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
			; //ToDo shoot (Add bullet to temp objects)
		l3d_delete_hits(&hits);
	}
}

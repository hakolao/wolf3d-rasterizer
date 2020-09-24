/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 16:37:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** ToDo: Might not work right yet :D
** ToDo: Apply changes to object transformation matrices
*/

static void		apply_transform_to_world(t_wolf3d *app, t_mat4 transform)
{
	int		i;
	int		j;

	i = -1;
	while (++i < app->active_scene->object_count)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->mesh_vertex_count)
		{
			ml_matrix4_mul_vec3(transform,
			app->active_scene->objects[i]->mesh_vertices[j]->position,
			app->active_scene->objects[i]->mesh_vertices[j]->position);
		}
		j = -1;
		while (++j < app->active_scene->objects[i]->mesh_triangle_count)
		{
			l3d_triangle_normal_set(
				&app->active_scene->objects[i]->mesh_triangles[j]);
			// ml_vector3_mul(app->active_scene->objects[i]->mesh_triangles[j].normal, -1,
			// 	app->active_scene->objects[i]->mesh_triangles[j].normal);
		}
	}
}

/*
** !Note: Diff axes have been switched to accommodate our chosen axes for the
** world & camera.
*/

static void		apply_movement(t_wolf3d *app, t_vec3 new_pos)
{
	t_player	*player;
	t_vec3		diff;
	t_mat4		translation;

	player = &app->player;
	ml_vector3_sub(player->pos, new_pos, diff);
	ml_matrix4_translation(diff[0], diff[1], diff[2], translation);
	apply_transform_to_world(app, translation);
	ft_memcpy(&player->pos, new_pos, sizeof(t_vec3));
}


void			init_player(t_wolf3d *app)
{
	ft_memcpy(&app->player.pos, &(t_vec3){0, 0, 0}, sizeof(t_vec3));
	ft_memcpy(&app->player.forward, &(t_vec3){0, 0, 1}, sizeof(t_vec3));
	ft_memcpy(&app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	app->player.speed = 0.5f;
	app->player.rot_speed = 0.1f;
}

/*
** !Note: Rotation axes have been switched to accommodate our chosen
** axes for the world & camera.
*/

void			rotate_player(t_wolf3d *app, t_vec3 axes)
{
	t_vec3	new_direction;
	t_mat4	rotation;
	t_player *player;

	player = &app->player;
	ml_vector3_mul(axes, ml_rad(player->rot_speed * app->delta_time), axes);
	ml_matrix4_rotation(axes[0], axes[1], axes[2], rotation);
	ml_matrix4_mul_vec3(rotation, player->forward, new_direction);
	apply_transform_to_world(app, rotation);
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
		ml_vector3_mul(player->forward, -player->speed * app->delta_time, add);
		ml_vector3_add(player->pos, add, new_pos);
	}
	else if (dir == move_backward)
	{
		ml_vector3_mul(player->forward, player->speed * app->delta_time, add);
		ml_vector3_add(player->pos, add, new_pos);
	}
	else if (dir == move_strafe_left)
	{
		ml_vector3_cross(player->forward, player->up, sideways);
		ml_vector3_mul(sideways, player->speed * app->delta_time, sideways);
		ml_vector3_add(player->pos, sideways, new_pos);
	}
	else if (dir == move_strafe_right)
	{
		ml_vector3_cross(player->forward, player->up, sideways);
		ml_vector3_mul(sideways, player->speed * app->delta_time, sideways);
		ml_vector3_sub(player->pos, sideways, new_pos);
	}
	apply_movement(app, new_pos);
}

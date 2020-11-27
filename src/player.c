/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:20:38 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/27 15:32:05 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			player_init(t_wolf3d *app, t_vec3 pos)
{
	ft_memcpy(app->player.pos, pos, sizeof(t_vec3));
	ft_memcpy(app->player.forward, &(t_vec3){0, 0, -1}, sizeof(t_vec3));
	ft_memcpy(app->player.up, &(t_vec3){0, 1, 0}, sizeof(t_vec3));
	ft_memcpy(app->player.sideways, &(t_vec3){1, 0, 0}, sizeof(t_vec3));
	app->player.speed = PLAYER_SPEED;
	app->player.rot_speed = PLAYER_ROTATION_SPEED;
	app->player.rot_x = 0;
	app->player.rot_y = 0;
	app->player.collider_radius = 0.3 * app->unit_size;
	app->player.collider_height = 0.8 * app->unit_size;
	ml_matrix4_id(app->player.rotation);
	ml_matrix4_id(app->player.inv_rotation);
	ml_matrix4_id(app->player.translation);
	ml_matrix4_id(app->player.inv_translation);
	player_grid_pos_update(app);
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

void			player_grid_pos_update(t_wolf3d *app)
{
	app->player.grid_pos[0] = -(app->player.pos[2] / app->unit_size / 2.0) + 0.5;
	app->player.grid_pos[1] = (app->player.pos[0] / app->unit_size / 2.0) + 0.5;
}

static t_bool	collider_ray_collides(t_kd_tree *triangle_tree,
					t_vec3 collider_origin, t_vec3 collider_dir,
					float collider_radius, t_vec3 hit_normal)
{
	t_vec3			add;
	t_vec3			point_on_collider;
	t_hits			*hits;
	t_hit			*closest_triangle_hit;
	t_vec3			point_dir;

	ml_vector3_mul(collider_dir, collider_radius, add);
	ml_vector3_add(collider_origin, add, point_on_collider);
	if (l3d_kd_tree_ray_hits(triangle_tree,
		collider_origin, collider_dir, &hits))
	{
		l3d_get_closest_hit(hits, &closest_triangle_hit);
		if (closest_triangle_hit != NULL)
		{
			ml_vector3_sub(point_on_collider,
				closest_triangle_hit->hit_point, point_dir);
			ml_vector3_normalize(point_dir, point_dir);
			if (ml_vector3_dot(closest_triangle_hit->normal, point_dir) <= 0)
			{
				ml_vector3_copy(closest_triangle_hit->normal, hit_normal);
				l3d_delete_hits(&hits);
				return (true);
			}
		}
		l3d_delete_hits(&hits);
	}
	return (false);
}

/*
** Not really a sphere, but a cube with flattened corners
*/

static t_bool	sphere_collides_with_triangles(t_kd_tree *triangle_tree,
					t_vec3 collider_origin, float collider_radius,
					t_vec3 hit_normal)
{
	t_vec3			dirs[6];
	t_mat4			rot_90xy;
	int32_t			i;

	ml_vector3_set(dirs[0], 1, 0, 0);
	ml_vector3_set(dirs[1], -1, 0, 0);
	ml_vector3_set(dirs[2], 0, 1, 0);
	ml_vector3_set(dirs[3], 0, -1, 0);
	ml_vector3_set(dirs[4], 0, 0, 1);
	ml_vector3_set(dirs[5], 0, 0, -1);
	ml_matrix4_rotation(ml_rad(90), ml_rad(90), 0, rot_90xy);
	i = -1;
	while (++i < 6)
	{
		if (collider_ray_collides(triangle_tree,
				collider_origin, dirs[i], collider_radius, hit_normal))
			return (true);
		ml_matrix4_mul_vec3(rot_90xy, dirs[i], dirs[i]);
		if (collider_ray_collides(triangle_tree,
				collider_origin, dirs[i], collider_radius, hit_normal))
			return (true);
	}
	return (false);
}

/*
** Player has two sphere colliders
*/

static t_bool	player_would_collide(t_wolf3d *app, t_vec3 add,
					t_vec3 hit_normal)
{
	t_vec3	new_pos_up;
	t_vec3	new_pos_down;

	ml_vector3_add((t_vec3){app->player.pos[0],
		app->player.pos[1] - app->player.collider_height / 2.0,
		app->player.pos[2]}, add, new_pos_up);
	ml_vector3_add((t_vec3){app->player.pos[0],
		app->player.pos[1] + app->player.collider_height / 2.0,
		app->player.pos[2]}, add, new_pos_down);
	return (sphere_collides_with_triangles(app->active_scene->triangle_tree,
		new_pos_down, app->player.collider_radius, hit_normal) ||
		sphere_collides_with_triangles(app->active_scene->triangle_tree,
		new_pos_up, app->player.collider_radius, hit_normal));
}

void			player_limit_movement(t_wolf3d *app, t_vec3 add)
{
	t_vec3		hit_normal;
	t_vec3		projected;

	if (player_would_collide(app, add, hit_normal))
	{
		ml_vector3_normalize(hit_normal, hit_normal);
		ml_vector3_mul(hit_normal, ml_vector3_dot(add, hit_normal), projected);
		ml_vector3_sub(add, projected, add);
	}
}

void			player_move(t_wolf3d *app, t_move dir)
{
	t_vec3		add;
	t_vec3		forward;
	t_vec3		sideways;
	t_mat4		rotation_x;
	float		speed;


	ft_memset(add, 0, sizeof(t_vec3));
	ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){0, 0, -1}, forward);
	ml_matrix4_mul_vec3(rotation_x, (t_vec3){1, 0, 0}, sideways);
	speed = app->player.is_running ? app->player.speed * 1.5 : app->player.speed;
	if (dir == move_forward)
		ml_vector3_mul(forward, speed * app->info.delta_time, add);
	else if (dir == move_backward)
		ml_vector3_mul(forward, -speed * app->info.delta_time, add);
	else if (dir == move_strafe_left)
		ml_vector3_mul(sideways, -speed * app->info.delta_time, add);
	else if (dir == move_strafe_right)
		ml_vector3_mul(sideways, speed * app->info.delta_time, add);
	player_limit_movement(app, add);
	ml_vector3_add(app->player.pos, add, app->player.pos);
	ml_matrix4_translation(app->player.pos[0],
		app->player.pos[1], app->player.pos[2], app->player.translation);
	ml_matrix4_inverse(app->player.translation, app->player.inv_translation);
	player_grid_pos_update(app);
}

void			player_shoot(t_wolf3d *app)
{
	t_vec3			origin;
	t_vec3			add;
	t_hits			*hits;
	t_hit			*closest_triangle_hit;

	if (app->active_scene->scene_id == scene_id_main_game)
	{
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
}

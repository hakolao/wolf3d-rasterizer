/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 14:09:54 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/17 18:07:14 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		scene_set_triangle_refs(t_scene *scene)
{
	int		i;
	int		j;
	int		k;
	int		num_triangles;

	i = -1;
	num_triangles = 0;
	k = 0;
	while (++i < (int)scene->num_objects)
	{
		j = -1;
		while (++j < scene->objects[i]->num_triangles)
		{
			scene->triangle_ref[k] = &scene->objects[i]->triangles[j];
			num_triangles++;
			k++;
		}
	}
	scene->num_triangles = num_triangles;
}

static void		set_scene_collision_tree(t_scene *scene, uint32_t num_objects)
{
	scene->bullet_tree = NULL;
	scene->num_objects = num_objects;
	if (scene->num_objects > 0)
	{
		scene_set_triangle_refs(scene);
		l3d_kd_tree_create_or_update(&scene->bullet_tree,
			scene->triangle_ref, scene->num_triangles);
	}
}

static void		read_and_init_scene_map(t_scene *scene,
					const char *map_filename)
{
	t_file_contents	*file;

	error_check(!(scene->map = malloc(sizeof(t_wolf3d_map))),
		"Failed to malloc map");
	error_check(!(scene->map->grid =
		malloc(sizeof(uint32_t) * MAP_SIZE * MAP_SIZE)),
		"Failed to malloc map grid");
	if (!(file = read_file(map_filename)))
		exit(EXIT_FAILURE);
	ft_memcpy(scene->map->grid, file->buf, file->size);
	destroy_file_contents(file);
	scene->map->size = MAP_SIZE;
}

static void		place_player(t_wolf3d *app, float unit_size, int32_t xy_rot[3])
{
	init_player(app,
		(t_vec3){(float)xy_rot[1] * (2 * unit_size) - unit_size, 0,
		(float)xy_rot[0] *  (2 * unit_size) - unit_size});
	rotate_player_horizontal(app, xy_rot[2]);
}

static void		place_dead_end_inaccessible(t_scene *scene, int32_t *obj_i,
							float unit_size, int32_t xy_rot[3])
{
	(void)scene;
	(void)obj_i;
	(void)unit_size;
	(void)xy_rot;
}

static void		place_dead_end(t_scene *scene, int32_t *obj_i,
							float unit_size, int32_t xy_rot[3])
{
	(void)scene;
	(void)obj_i;
	(void)unit_size;
	(void)xy_rot;
}

static void		place_corridor(t_scene *scene, int32_t *obj_i,
							float unit_size, int32_t xy_rot[3])
{
	(void)scene;
	(void)obj_i;
	(void)unit_size;
	(void)xy_rot;
}

static void		place_wall(t_scene *scene, int32_t *obj_i,
							float unit_size, int32_t xy_rot[3])
{
	(void)scene;
	(void)obj_i;
	(void)unit_size;
	(void)xy_rot;
}

static void		place_corner(t_scene *scene, int32_t *obj_i,
							float unit_size, int32_t xy_rot[3])
{
	(void)scene;
	(void)obj_i;
	(void)unit_size;
	(void)xy_rot;
}

static void		place_floor(t_scene *scene, int32_t *obj_i,
							float unit_size, int32_t xy_rot[3])
{
	scene->objects[*obj_i] =
		l3d_read_obj("assets/models/room_tiles/room_floor.obj",
					"assets/textures/test_texture_small.bmp");
	l3d_3d_object_scale(scene->objects[*obj_i], unit_size, unit_size, unit_size);
	l3d_3d_object_translate(scene->objects[*obj_i],
		(float)xy_rot[1] * (2 * unit_size), PLAYER_HEIGHT * 1,
		-(float)xy_rot[0] * (2 * unit_size));
	(*obj_i)++;
}

void			read_map_to_scene(t_wolf3d *app,
						t_scene *scene, const char *map_filename)
{
	int32_t			x;
	int32_t			y;
	uint32_t		cell;
	int32_t			obj_i;

	read_and_init_scene_map(scene, map_filename);
	y = -1;
	obj_i = 0;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
		{
			cell = scene->map->grid[y * MAP_SIZE + x];
			if (!(cell & m_room))
				continue; 
			if ((cell & m_start))
				place_player(app, app->window->width, (int32_t[3]){x, y, -140});
			if ((cell & p_dead_up) == p_dead_up)
				place_dead_end(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 0});
			else if ((cell & p_dead_right) == p_dead_right)
				place_dead_end(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 90});
			else if ((cell & p_dead_down) == p_dead_down)
				place_dead_end(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 180});
			else if ((cell & p_dead_left) == p_dead_left)
				place_dead_end(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 270});
			else if ((cell & p_corr_horz) == p_corr_horz)
				place_corridor(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 90});
			else if ((cell & p_corr_vert) == p_corr_vert)
				place_corridor(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 0});
			else if ((cell & p_wall_up) == p_wall_up)
				place_wall(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 0});
			else if ((cell & p_wall_right) == p_wall_right)
				place_wall(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 90});
			else if ((cell & p_wall_down) == p_wall_down)
				place_wall(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 180});
			else if ((cell & p_wall_left) == p_wall_left)
				place_wall(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 270});
			else if ((cell & p_corner_se) == p_corner_se)
				place_corner(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 0});
			else if ((cell & p_corner_sw) == p_corner_sw)
				place_corner(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 90});
			else if ((cell & p_corner_nw) == p_corner_nw)
				place_corner(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 180});
			else if ((cell & p_corner_ne) == p_corner_ne)
				place_corner(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 270});
			else if ((cell & p_dead_all) == p_dead_all)
				place_dead_end_inaccessible(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 0});
			else if ((cell & p_middle_floor) == p_middle_floor)
				place_floor(scene, &obj_i, app->window->width, (int32_t[3]){x, y, 0});
		}
	}
	set_scene_collision_tree(scene, obj_i);
}

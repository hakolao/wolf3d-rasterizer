/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 14:09:54 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/16 14:49:04 by ohakola+vei      ###   ########.fr       */
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

static void		place_floor(t_scene *scene, int32_t obj_i,
							float unit_size, int32_t xy[2])
{
	scene->objects[obj_i] =
		l3d_read_obj("assets/icosphere_massive2.obj",
					"assets/textures/test_texture.bmp",
					"assets/textures/test_texture_normal.bmp");
	l3d_3d_object_scale(scene->objects[obj_i], unit_size * 10, unit_size * 10, unit_size * 10);
	l3d_3d_object_translate(scene->objects[obj_i],
		xy[1] * unit_size * 2, PLAYER_HEIGHT * 1,
		-xy[0] * unit_size * 2);
}

static void		place_player(t_wolf3d *app, float unit_size, int32_t xy[2])
{
	init_player(app,
		(t_vec3){(float)xy[1] * unit_size - (float)unit_size / 2.0, 0,
		(float)xy[0] * unit_size - (float)unit_size / 2.0});
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
			if ((cell & c_floor_start))
				place_player(app, app->window->width, (int32_t[2]){x, y});
			if ((cell & c_floor))
				place_floor(scene, obj_i++, app->window->width,
					(int32_t[2]){x, y});
			// ToDo Read other parts too, walls etc. Clean up.
		}
	}
	set_scene_collision_tree(scene, obj_i);
}

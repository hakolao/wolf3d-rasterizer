/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_generate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/09 14:35:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void				scene_set_triangle_refs(t_scene *scene)
{
	int		i;
	int		j;
	int		k;
	int		num_triangles;

	if (scene->triangle_ref != NULL)
		free(scene->triangle_ref);
	i = -1;
	num_triangles = 0;
	while (++i < (int)scene->num_objects)
		num_triangles += scene->objects[i]->num_triangles;
	scene->num_triangles = num_triangles;
	error_check(!(scene->triangle_ref =
		malloc(sizeof(t_triangle*) * num_triangles)),
		"Failed to malloc triangle ref");
	i = -1;
	k = 0;
	while (++i < (int)scene->num_objects)
	{
		j = -1;
		while (++j < scene->objects[i]->num_triangles)
			scene->triangle_ref[k++] = &scene->objects[i]->triangles[j];
	}
}

static void				set_scene_collision_tree(t_scene *scene)
{
	scene->triangle_tree = NULL;
	if (scene->num_objects > 0)
	{
		scene_set_triangle_refs(scene);
		l3d_kd_tree_create_or_update(&scene->triangle_tree,
			scene->triangle_ref, scene->num_triangles);
	}
}

/*
** (1 << i) corresponds to model keys. ToDo: Refactor scene to contain list of
** model keys to iterate over.
*/

static void				instantiate_cell_features(t_wolf3d *app,
							uint32_t cell, int32_t *obj_i, int32_t xy[2])
{
	t_3d_object		*model;
	int32_t			i;
	t_3d_object		*obj;

	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 8)
	{
		if ((cell & (1 << i)))
		{
			if ((model = hash_map_get(app->active_scene->models, 1 << i)))
			{
				app->active_scene->objects[*obj_i] =
					l3d_object_instantiate(model, app->unit_size,
					(1 << i) == c_block_ne || (1 << i) == c_block_nw ||
					(1 << i) == c_block_se || (1 << i) == c_block_sw);
				obj = app->active_scene->objects[*obj_i];
				l3d_3d_object_rotate(obj, 90, 90, 0);
				l3d_3d_object_translate(obj,
					(float)xy[1] * (2 * app->unit_size),
						app->unit_size, -(float)xy[0] * (2 * app->unit_size));
				l3d_object_set_shading_opts(obj, e_shading_depth);
				(*obj_i)++;
			}
		}
	}
}

/*
** Based on map data, generates map 3d objects from scene's model table.
** Models are copied and instantiated.
*/

void					scene_objects_generate(t_wolf3d *app, t_scene *scene)
{
	int32_t			x;
	int32_t			y;
	uint32_t		cell;
	int32_t			obj_i;
	t_bool			player_placed;

	y = -1;
	obj_i = 0;
	player_placed = false;
	while (++y < scene->map->size)
	{
		x = -1;
		while (++x < scene->map->size)
		{
			cell = scene->map->grid[y * scene->map->size + x];
			if (!(cell & m_room))
				continue;
			if ((cell & m_start) || (!player_placed && (player_placed = true)))
				place_player(app, app->unit_size, (int32_t[3]){x, y, 0});
			instantiate_cell_features(app, cell, &obj_i, (int32_t[2]){x, y});
		}
	}
	scene->num_objects = obj_i;
	set_scene_collision_tree(scene);
	l3d_skybox_create(scene->skybox, scene->skybox_textures, app->unit_size);
}

void					scene_map_init(t_scene *scene)
{
	t_file_contents	*file;
	char			header[4];

	if (!(file = read_file(scene->map_filename)))
		exit(EXIT_FAILURE);
	error_check(!(scene->map = malloc(sizeof(t_wolf3d_map))),
		"Failed to malloc map");
	ft_memcpy(header, file->buf, 4);
	if (!ft_strequ(header, "MAP\0"))
		error_check(true,
			"Invalid file, not a map file. First 4 bytes must be MAP\0");
	ft_memcpy(&scene->map->size, file->buf + 4, 4);
	error_check(!(scene->map->grid =
		malloc(sizeof(uint32_t) * scene->map->size * scene->map->size)),
		"Failed to malloc map grid");
	ft_memcpy(scene->map->grid, file->buf + 8, file->size - 8);
	destroy_file_contents(file);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 14:09:54 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/19 19:50:04 by ohakola+vei      ###   ########.fr       */
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

static void		instantiate_3d_model(t_3d_object *new_obj, t_3d_object *model,
							float unit_size, int32_t xy[2])
{
	new_obj = l3d_3d_object_create(model->num_vertices, model->num_triangles);
	ft_memcpy(new_obj, model, sizeof(*model));
	l3d_3d_object_scale(new_obj,
		unit_size, unit_size,
		unit_size);
	l3d_3d_object_translate(new_obj,
		(float)xy[1] * (2 * unit_size), PLAYER_HEIGHT * 1,
		-(float)xy[0] * (2 * unit_size));
}

static void		instantiate_cell_features(t_wolf3d *app,
					uint32_t cell, int32_t *obj_i, int32_t xy[2])
{
	t_3d_object		*model;
	uint32_t		model_key;

	model_key = 1;
	while (model_key != UINT32_MAX)
	{
		if ((cell & model_key))
		{
			if ((model = hash_map_get(app->active_scene->models, model_key)))
			{
				instantiate_3d_model(app->active_scene->objects[*obj_i],
					model, app->window->width, xy);
				obj_i++;
			}
		}
		model_key <<= 1;
	}
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
			instantiate_cell_features(app, cell, &obj_i, (int32_t[2]){x, y});
		}
	}
	set_scene_collision_tree(scene, obj_i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 14:09:54 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/24 13:38:56 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_3d_object		*plane_create(t_surface	*texture)
{
	t_3d_object		*plane;

	plane = l3d_3d_object_create(4, 2);
	if (texture != NULL)
	{
		plane->material->height = texture->h;
		plane->material->width = texture->w;
		plane->material->texture = texture->pixels;
	}
	ml_vector4_copy((t_vec4){-1, 0, -1, 1}, plane->vertices[0]->pos);
	ml_vector4_copy((t_vec4){-1, 0, 1, 1}, plane->vertices[1]->pos);
	ml_vector4_copy((t_vec4){1, 0, 1, 1}, plane->vertices[2]->pos);
	ml_vector4_copy((t_vec4){1, 0, -1, 1}, plane->vertices[3]->pos);
	plane->triangles[0].vtc_indices[0] = 0;
	plane->triangles[0].vtc_indices[1] = 1;
	plane->triangles[0].vtc_indices[2] = 2;
	plane->triangles[1].vtc_indices[0] = 0;
	plane->triangles[1].vtc_indices[1] = 2;
	plane->triangles[1].vtc_indices[1] = 3;
	l3d_triangle_set(&plane->triangles[0], (t_vertex*[3]){
		plane->vertices[0], plane->vertices[1], plane->vertices[2]}, plane);
	l3d_triangle_set(&plane->triangles[1], (t_vertex*[3]){
		plane->vertices[0], plane->vertices[2], plane->vertices[3]}, plane);
	// plane->triangles[0].is_single_sided = false;
	// plane->triangles[1].is_single_sided = false;
	ml_vector2_copy((t_vec2){0, 0}, plane->triangles[0].uvs[0]);
	ml_vector2_copy((t_vec2){1, 0}, plane->triangles[0].uvs[1]);
	ml_vector2_copy((t_vec2){1, 1}, plane->triangles[0].uvs[2]);
	ml_vector2_copy((t_vec2){0, 0}, plane->triangles[1].uvs[0]);
	ml_vector2_copy((t_vec2){1, 1}, plane->triangles[1].uvs[1]);
	ml_vector2_copy((t_vec2){0, 1}, plane->triangles[1].uvs[2]);
	return (plane);
}

static void				generate_skybox(t_scene *scene, float unit_size)
{
	int32_t		i;

	i = -1;
	while (++i < 6)
	{
		scene->skybox[i] = plane_create(scene->skybox_textures[i]);
		l3d_3d_object_scale(scene->skybox[i],
			unit_size * 10, unit_size * 10, unit_size * 10);
	}
	//front
	l3d_3d_object_rotate(scene->skybox[0], 90, 0, -90);
	l3d_3d_object_translate(scene->skybox[0], 0, 0, -unit_size * 10);
	//left
	l3d_3d_object_rotate(scene->skybox[1], 0, 0, -90);
	l3d_3d_object_translate(scene->skybox[1], -unit_size * 10, 0, 0);
	//top
	l3d_3d_object_rotate(scene->skybox[2], 0, 180, 0);
	l3d_3d_object_translate(scene->skybox[2], 0, -unit_size * 10, 0);
	//back
	// l3d_3d_object_rotate(scene->skybox[3], 180, 0, 0);
	l3d_3d_object_translate(scene->skybox[3], 0, 0, unit_size * 10);
	//left
	l3d_3d_object_rotate(scene->skybox[4], 0, 180, 90);
	l3d_3d_object_translate(scene->skybox[4], unit_size * 10, 0, 0);
	//bottom
	l3d_3d_object_translate(scene->skybox[5], 0, unit_size * 10, 0);
	l3d_3d_object_rotate(scene->skybox[5], 0, 90, 0);
}

static void				scene_set_triangle_refs(t_scene *scene)
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

static void				set_scene_collision_tree(t_scene *scene)
{
	scene->bullet_tree = NULL;
	if (scene->num_objects > 0)
	{
		scene_set_triangle_refs(scene);
		l3d_kd_tree_create_or_update(&scene->bullet_tree,
			scene->triangle_ref, scene->num_triangles);
	}
}

static void				place_player(t_wolf3d *app, float unit_size, int32_t xy_rot[3])
{
	init_player(app,
		(t_vec3){(float)xy_rot[1] * (2 * unit_size), 0,
		(float)xy_rot[0] *  (2 * unit_size)});
	rotate_player_horizontal(app, xy_rot[2]);
}

static t_3d_object		*instantiate_3d_model(t_3d_object *model,
							float unit_size, int32_t xy[2])
{
	t_3d_object	*new_obj;

	new_obj = l3d_3d_object_copy(model);
	l3d_3d_object_scale(new_obj, unit_size, unit_size, unit_size);
	l3d_3d_object_translate(new_obj,
		(float)xy[1] * (2 * unit_size), PLAYER_HEIGHT * 1,
		-(float)xy[0] * (2 * unit_size));
	return (new_obj);
}

static void				instantiate_cell_features(t_wolf3d *app,
							uint32_t cell, int32_t *obj_i, int32_t xy[2])
{
	t_3d_object		*model;
	int32_t			i;
	uint32_t		key;

	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 4)
	{
		key = 1 << i;
		if ((cell & key))
		{
			if ((model = hash_map_get(app->active_scene->models, key)))
			{
				app->active_scene->objects[*obj_i] =
					instantiate_3d_model(model, app->window->width, xy);
				(*obj_i)++;
			}
		}
	}
}

/*
** Based on map data, generates map 3d objects from scene's model table.
** Models are copied and instantiated.
*/

void			generate_scene_objects(t_wolf3d *app,
						t_scene *scene)
{
	int32_t			x;
	int32_t			y;
	uint32_t		cell;
	int32_t			obj_i;

	y = -1;
	obj_i = 0;
	while (++y < scene->map->size)
	{
		x = -1;
		while (++x < scene->map->size)
		{
			cell = scene->map->grid[y * scene->map->size + x];
			if (!(cell & m_room))
				continue;
			if ((cell & m_start))
				place_player(app, app->window->width, (int32_t[3]){x, y, 0});
			instantiate_cell_features(app, cell, &obj_i, (int32_t[2]){x, y});
		}
	}
	scene->num_objects = obj_i;
	set_scene_collision_tree(scene);
	generate_skybox(scene, app->window->width);
}

void				read_and_init_scene_map(t_scene *scene)
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

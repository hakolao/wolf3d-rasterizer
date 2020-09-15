/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:28:44 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 13:14:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void				scene_vertices_init(t_wolf3d *app, t_scene *scene)
{
	int		i;
	int		j;
	t_mat4	translation;
	float	scale;

	if (app->active_scene->main_camera == NULL)
		return ;
	ml_matrix4_translation(app->active_scene->main_camera->screen_dist, 0, 0,
		translation);
	scale = app->main_window->height / 2.0 - 1;
	i = -1;
	while (++i < scene->object_count)
	{
		j = -1;
		while (++j < scene->objects[i]->mesh_vertex_count)
		{
			ml_vector3_mul(scene->objects[i]->mesh_vertices[j]->position,
				scale, scene->objects[i]->mesh_vertices[j]->position);
			ml_matrix4_mul_vec3(translation,
			scene->objects[i]->mesh_vertices[j]->position,
			scene->objects[i]->mesh_vertices[j]->position);
		}
		j = -1;
		while (++j < scene->objects[i]->mesh_triangle_count)
		{
			l3d_triangle_normal_set(&scene->objects[i]->mesh_triangles[j]);
			ml_vector3_mul(scene->objects[i]->mesh_triangles[j].normal, -1,
				scene->objects[i]->mesh_triangles[j].normal);
		}
	}
}

/*
** Add assets as .obj files to scenes
*/

t_3d_object		**create_scene1_objects(int32_t *object_count)
{
	t_3d_object	**objects;

	*object_count = 1;
	error_check(!(objects = malloc(sizeof(*objects) * *object_count)),
		"Failed to malloc objects for test scene");
	objects[0] = read_object_file("assets/icosphere.obj");
	return (objects);
}

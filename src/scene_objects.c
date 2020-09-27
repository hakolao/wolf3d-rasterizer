/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:28:44 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 17:51:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			update_world_scale(t_scene *scene, t_mat4 scale)
{
	int				i;
	t_mat4			new_scale;
	t_mat4			inverse;

	ml_matrix4_inverse(scene->world_scale, inverse);
	ml_matrix4_mul(scene->world_scale, scale, new_scale);
	i = -1;
	while (++i < scene->object_count)
	{
		transform_3d_object(scene->objects[i], inverse);
		transform_3d_object(scene->objects[i], new_scale);
	}
	ft_memcpy(scene->world_scale, new_scale, sizeof(t_mat4));
}

void			update_world_rotation(t_scene *scene, t_mat4 rotation)
{
	int				i;
	t_mat4			new_rotation;
	t_mat4			inverse;

	ml_matrix4_inverse(scene->world_rotation, inverse);
	ml_matrix4_mul(scene->world_rotation, rotation, new_rotation);
	i = -1;
	while (++i < scene->object_count)
	{
		transform_3d_object(scene->objects[i], inverse);
		transform_3d_object(scene->objects[i], new_rotation);
	}
	ft_memcpy(scene->world_rotation, new_rotation, sizeof(t_mat4));
}

void			update_world_translation(t_scene *scene, t_mat4 translation)
{
	int				i;
	t_mat4			new_translation;
	t_mat4			inverse;

	ml_matrix4_inverse(scene->world_translation, inverse);
	ml_matrix4_mul(scene->world_translation, translation, new_translation);
	i = -1;
	while (++i < scene->object_count)
	{
		transform_3d_object(scene->objects[i], inverse);
		transform_3d_object(scene->objects[i], new_translation);
	}
	ft_memcpy(scene->world_translation, new_translation, sizeof(t_mat4));
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:28:44 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 18:22:50 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			update_world_transform(t_scene *scene)
{
	int				i;
	t_mat4			tmp;
	t_mat4			transform;
	t_mat4			inverse;

	ml_matrix4_inverse(scene->world_transform, inverse);
	ml_matrix4_mul(scene->world_translation, scene->world_scale, tmp);
	ml_matrix4_mul(scene->world_rotation, tmp, transform);
	i = -1;
	while (++i < (int)scene->object_count)
	{
		transform_3d_object(scene->objects[i], inverse);
		transform_3d_object(scene->objects[i], transform);
	}
	ft_memcpy(scene->world_transform, transform, sizeof(t_mat4));
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

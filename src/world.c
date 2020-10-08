/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:28:44 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/08 15:09:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			update_world_rotation(t_scene *scene, t_mat4 new_rotation)
{
	int				i;
	t_mat4			new_world_rotation;

	ml_matrix4_mul(scene->world_rotation, new_rotation, new_world_rotation);
	ft_memcpy(scene->world_rotation, new_world_rotation, sizeof(t_mat4));
	i = -1;
	while (++i < (int)scene->num_objects)
		l3d_3d_object_transform(scene->objects[i], new_rotation);
}

void			update_world_scale(t_scene *scene, t_mat4 new_scale)
{
	int				i;
	t_mat4			inverse_translation;
	t_mat4			new_world_scale;

	ml_matrix4_inverse(scene->world_translation, inverse_translation);
	ml_matrix4_mul(scene->world_scale, new_scale, new_world_scale);
	ft_memcpy(scene->world_scale, new_world_scale, sizeof(t_mat4));
	i = -1;
	while (++i < (int)scene->num_objects)
	{
		l3d_3d_object_transform(scene->objects[i], inverse_translation);
		l3d_3d_object_transform(scene->objects[i], new_world_scale);
		l3d_3d_object_transform(scene->objects[i], scene->world_translation);
	}
}

void			update_world_translation(t_scene *scene, t_mat4	new_translation)
{
	int				i;
	t_mat4			new_world_translation;

	ml_matrix4_mul(scene->world_translation, new_translation, new_world_translation);
	ft_memcpy(scene->world_translation, new_world_translation, sizeof(t_mat4));
	i = -1;
	while (++i < (int)scene->num_objects)
		l3d_3d_object_transform(scene->objects[i], new_translation);
}

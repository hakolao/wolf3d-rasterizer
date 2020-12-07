/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 02:21:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		scene_textures_destroy(t_scene *scene)
{
	t_surface	*texture;
	int32_t		i;

	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 8)
	{
		if ((texture = hash_map_get(scene->textures, 1 << i)))
			free(texture->pixels);
	}
	hash_map_destroy_free(scene->textures);
}

void		scene_normal_maps_destroy(t_scene *scene)
{
	t_surface	*normal_map;
	int32_t		i;

	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 8)
	{
		if ((normal_map = hash_map_get(scene->normal_maps, 1 << i)))
			free(normal_map->pixels);
	}
	hash_map_destroy_free(scene->normal_maps);
}

void		scene_models_destroy(t_scene *scene)
{
	t_3d_object	*model;
	int32_t		i;

	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 8)
		if ((model = hash_map_get(scene->models, 1 << i)))
			l3d_3d_object_destroy(model);
	hash_map_destroy(scene->models);
}

void		scene_skybox_destroy(t_scene *scene)
{
	int32_t		i;

	i = -1;
	while (++i < 6)
	{
		l3d_3d_object_destroy(scene->skybox[i]);
		free(scene->skybox_textures[i]->pixels);
		free(scene->skybox_textures[i]);
	}
}

void		scene_objects_destroy(t_scene *scene)
{
	int32_t		i;

	i = -1;
	while (++i < (int)scene->num_objects)
		l3d_3d_object_destroy(scene->objects[i]);
}

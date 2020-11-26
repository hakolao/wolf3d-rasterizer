/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:55:05 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/26 14:55:32 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			scene_assets_load(t_scene *scene, t_scene_data *data)
{
	int32_t		i;
	uint32_t	key;

	scene->models = hash_map_create(data->num_assets_to_load);
	scene->textures = hash_map_create(data->num_assets_to_load);
	i = -1;
	while (++i < (int32_t)data->num_assets_to_load)
	{
		key = data->asset_keys[i];
		if (data->texture_files[i] != NULL && data->model_files[i] != NULL)
		{
			hash_map_add(scene->textures, key,
				l3d_read_bmp_image_32bit_rgba_surface(data->texture_files[i]));
			hash_map_add(scene->models, key,
				l3d_read_obj(data->model_files[i],
					hash_map_get(scene->textures, key)));
		}
	}
	scene->skybox_textures[0] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/front.bmp");
	scene->skybox_textures[1] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/left.bmp");
	scene->skybox_textures[2] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/top.bmp");
	scene->skybox_textures[3] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/back.bmp");
	scene->skybox_textures[4] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/right.bmp");
	scene->skybox_textures[5] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/bottom.bmp");
	// Temp objects should not share materials with other objects,
	// Their texture gets deleted when they are cleared.
	hash_map_add(scene->models, (int)"bullet_hole",
		l3d_plane_create(l3d_read_bmp_image_32bit_rgba_surface(
			"assets/textures/bullet_hole.bmp")));
}

void			scene_minimap_init(t_wolf3d *app)
{
	float				minimap_render_size;

	map_init_image_assets(&app->active_scene->map->map_images);
	minimap_render_size = app->window->height * 0.25;
	map_render_resize(app->active_scene->map, minimap_render_size,
		(t_vec2){app->window->width - minimap_render_size - 10, 10});
}

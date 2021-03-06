/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 02:20:15 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		scene_set_skybox_textures(t_scene *scene)
{
	scene->skybox_textures[0] = l3d_read_bmp_32bit_rgba_surface(
		"assets/skybox/front.bmp");
	scene->skybox_textures[1] = l3d_read_bmp_32bit_rgba_surface(
		"assets/skybox/left.bmp");
	scene->skybox_textures[2] = l3d_read_bmp_32bit_rgba_surface(
		"assets/skybox/top.bmp");
	scene->skybox_textures[3] = l3d_read_bmp_32bit_rgba_surface(
		"assets/skybox/back.bmp");
	scene->skybox_textures[4] = l3d_read_bmp_32bit_rgba_surface(
		"assets/skybox/right.bmp");
	scene->skybox_textures[5] = l3d_read_bmp_32bit_rgba_surface(
		"assets/skybox/bottom.bmp");
}

void			scene_assets_load(t_scene *scene, t_scene_data *data)
{
	int32_t		i;
	uint32_t	key;

	scene->models = hash_map_create(32);
	scene->textures = hash_map_create(32);
	scene->normal_maps = hash_map_create(32);
	i = -1;
	while (++i < (int32_t)data->num_assets_to_load)
	{
		key = data->asset_keys[i];
		if (data->texture_files[i] != NULL)
			hash_map_add(scene->textures, key,
				l3d_read_bmp_32bit_rgba_surface(data->texture_files[i]));
		if (data->normal_map_files[i] != NULL)
			hash_map_add(scene->normal_maps, key,
				l3d_read_bmp_32bit_rgba_surface(data->normal_map_files[i]));
		if (data->model_files[i] != NULL)
		{
			hash_map_add(scene->models, key,
				l3d_read_obj(data->model_files[i],
					hash_map_get(scene->textures, key),
					hash_map_get(scene->normal_maps, key)));
		}
	}
	scene_set_skybox_textures(scene);
}

void			scene_minimap_init(t_wolf3d *app)
{
	float				minimap_render_size;

	map_init_image_assets(&app->active_scene->map->map_images);
	minimap_render_size = app->window->height * 0.8;
	map_render_resize(app->active_scene->map, minimap_render_size,
		(t_vec2){app->window->width - app->window->height * 0.3 - 10, 10});
}

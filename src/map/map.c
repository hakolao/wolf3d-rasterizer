/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 23:26:10 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			map_rescale_image_assets(t_wolf3d_map *map)
{
	float		size;
	int32_t		i;
	t_surface	*curr_image;
	uint32_t	key;

	size = ceil(map->cell_render_size);
	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 8)
	{
		key = 1 << i;
		if ((curr_image = hash_map_get(map->map_images, key)))
		{
			hash_map_add(map->map_images, key,
				l3d_image_scaled(curr_image, size, size));
			free(curr_image->pixels);
			free(curr_image);
		}
	}
}

void			map_init_image_assets(t_hash_table **map_images)
{
	error_check(!(*map_images = hash_map_create(10)),
		"Failed to create hash table");
	hash_map_add(*map_images, c_floor,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/floor.bmp"));
	hash_map_add(*map_images, c_floor_start,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/floor_start.bmp"));
	hash_map_add(*map_images, c_wall_up,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_up.bmp"));
	hash_map_add(*map_images, c_wall_right,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_right.bmp"));
	hash_map_add(*map_images, c_wall_down,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_down.bmp"));
	hash_map_add(*map_images, c_wall_left,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_left.bmp"));
	hash_map_add(*map_images, c_block_nw,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_nw.bmp"));
	hash_map_add(*map_images, c_block_ne,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_ne.bmp"));
	hash_map_add(*map_images, c_block_se,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_se.bmp"));
	hash_map_add(*map_images, c_block_sw,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_sw.bmp"));
}

void			map_render_resize(t_wolf3d_map *map,
					float new_size, t_vec2 new_pos)
{
	map->render_size = new_size;
	map->cell_render_size = map->render_size / (float)map->size;
	ml_vector2_copy(new_pos, map->render_pos);
	map_rescale_image_assets(map);
}

void			map_destroy(t_wolf3d_map *map)
{
	int32_t		i;
	t_surface	*image;
	uint32_t	key;

	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 8)
	{
		key = 1 << i;
		image = hash_map_get(map->map_images, key);
		if (image)
		{
			free(image->pixels);
			free(image);
		}
	}
	hash_map_destroy(map->map_images);
	free(map->grid);
	free(map);
	map = NULL;
}

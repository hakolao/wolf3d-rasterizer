/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:47:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 00:59:34 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		player_render(t_wolf3d_map *map,
					t_framebuffer *framebuffer, t_player *player)
{
	t_surface	player_image;
	t_vec2		pos;
	float		size;
	uint32_t	pixels[16];
	int32_t		i;

	size = 4;
	player_image.h = size;
	player_image.w = size;
	i = -1;
	while (++i < 16)
		pixels[i] = 0x00ff00ff;
	player_image.pixels = pixels;
	if (player->grid_pos[0] >= 0 && player->grid_pos[0] < map->size &&
		player->grid_pos[1] >= 0 && player->grid_pos[1] < map->size)
	{
		ml_vector2_copy((t_vec2){map->render_pos[0] +
			player->grid_pos[0] * map->cell_render_size,
			map->render_pos[1] + player->grid_pos[1] * map->cell_render_size},
			pos);
		l3d_image_place(&(t_surface){.h = framebuffer->height,
			.w = framebuffer->width, .pixels = framebuffer->buffer},
				&player_image, (int32_t[2]){pos[0], pos[1]}, 1.0);
		player_dir_render(framebuffer, player, pos, size);
	}
}

void			map_minimap_render_full(t_wolf3d_map *map,
					t_framebuffer *framebuffer, t_player *player)
{
	int32_t		x;
	int32_t		y;
	t_surface	*image;

	y = -1;
	while (++y < map->size)
	{
		x = -1;
		while (++x < map->size)
		{
			if ((map->grid[y * map->size + x] & c_floor))
			{
				if ((image = hash_map_get(map->map_images, c_floor)))
				{
					l3d_image_place(&(t_surface){.h = framebuffer->height,
					.w = framebuffer->width, .pixels = framebuffer->buffer},
					image, (int32_t[2]){
					(int32_t)(map->render_pos[0] + x * map->cell_render_size),
					(int32_t)(map->render_pos[1] + y * map->cell_render_size),
					}, 1.0);
				}
			}
		}
	}
	player_render(map, framebuffer, player);
}

static void		partial_minimap_player_render(t_wolf3d_map *map,
					t_framebuffer *framebuffer, float minimap_size,
					t_player *player)
{
	t_surface	player_image;
	t_vec2		player_render_pos;
	float		size;
	uint32_t	pixels[16];
	int32_t		i;

	size = 4;
	player_image.h = size;
	player_image.w = size;
	i = -1;
	while (++i < 16)
		pixels[i] = 0x00ff00ff;
	player_image.pixels = pixels;
	player_render_pos[0] =
		map->render_pos[0] + player->grid_pos[0] * map->cell_render_size
		- player->grid_pos[0] * map->cell_render_size + minimap_size / 2.0;
	player_render_pos[1] =
		map->render_pos[1] + player->grid_pos[1] * map->cell_render_size
		- player->grid_pos[1] * map->cell_render_size + minimap_size / 2.0;
	l3d_image_place(&(t_surface){.h = framebuffer->height,
		.w = framebuffer->width, .pixels = framebuffer->buffer}, &player_image,
		(int32_t[2]){player_render_pos[0], player_render_pos[1]}, 1.0);
	player_dir_render(framebuffer, player, player_render_pos, size);
}

static void		place_map_image(t_wolf3d_map *map, t_surface *minimap,
					float minimap_size, t_vec4 player_pos_xy)
{
	t_surface	*image;

	if ((image = hash_map_get(map->map_images, c_floor)))
	{
		l3d_image_place(minimap, image,
			(int32_t[2]){(int32_t)(player_pos_xy[2] * map->cell_render_size
			- player_pos_xy[0] * map->cell_render_size
			+ minimap_size / 2.0),
			(int32_t)(player_pos_xy[3] * map->cell_render_size
			- player_pos_xy[1] * map->cell_render_size
			+ minimap_size / 2.0)}, 1.0);
	}
}

void			map_minimap_render_partial(t_wolf3d_map *map,
					t_framebuffer *f, float size,
					t_player *player)
{
	int32_t		xy[2];
	t_surface	minimap;
	uint32_t	minimap_pixels[(int32_t)size * (int32_t)size];
	int32_t		pixels_size;

	pixels_size = (int32_t)size * (int32_t)size;
	minimap.w = (uint32_t)size;
	minimap.h = (uint32_t)size;
	minimap.pixels = minimap_pixels;
	xy[0] = -1;
	while (++xy[0] < pixels_size)
		minimap.pixels[xy[0]] = 0x000000ff;
	xy[1] = -1;
	while (++xy[1] < map->size)
	{
		xy[0] = -1;
		while (++xy[0] < map->size)
			if ((map->grid[xy[1] * map->size + xy[0]] & c_floor))
				place_map_image(map, &minimap, size, (t_vec4){
					player->grid_pos[0], player->grid_pos[1], xy[0], xy[1]});
	}
	l3d_image_place(&(t_surface){.h = f->height, .w = f->width,
		.pixels = f->buffer}, &minimap, (int32_t[2]){map->render_pos[0],
		map->render_pos[1]}, 0.5);
	partial_minimap_player_render(map, f, size, player);
}

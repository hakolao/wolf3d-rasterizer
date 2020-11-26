/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 14:01:04 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/26 16:31:49 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			map_boundary_render(t_wolf3d_map *map, t_framebuffer *framebuffer,
					uint32_t grid_color)
{
	int32_t		i;
	float		interval;

	interval = map->cell_render_size;
	i = 0;
	while (i <= map->size)
	{
		l3d_line_draw(framebuffer->buffer,
			(uint32_t[2]){framebuffer->width, framebuffer->height
		}, (int32_t[2][2]){{map->render_pos[0] + (float)(i * interval),
			map->render_pos[1]},
			{map->render_pos[0] + (float)(i * interval),
				map->render_pos[1] +
				map->render_size}}, grid_color);
		l3d_line_draw(framebuffer->buffer,
			(uint32_t[2]){framebuffer->width, framebuffer->height
		}, (int32_t[2][2]){{map->render_pos[0],
			map->render_pos[1] + (float)(i * interval)},
			{map->render_pos[0] + map->render_size,
			map->render_pos[1] + (float)(i * interval)}},
			grid_color);
		i += map->size;
	}
}

void			map_features_render(t_wolf3d_map *map, t_framebuffer *framebuffer)
{
	int32_t		x;
	int32_t		y;
	uint32_t	feature_flag;
	int32_t		i;
	t_surface	*image;

	y = -1;
	while (++y < map->size)
	{
		x = -1;
		while (++x < map->size)
		{
			i = -1;
			while (++i < 32)
			{
				feature_flag = 1 << i;
				if ((map->grid[y * map->size + x] & feature_flag))
				{
					image = hash_map_get(map->map_images, feature_flag);
					if (image != NULL)
						l3d_image_place(
						&(t_surface){
						.h = framebuffer->height,
						.w = framebuffer->width,
						.pixels = framebuffer->buffer},
						image,
						(int32_t[2]){
						(int32_t)map->render_pos[0] + x * map->cell_render_size,
						(int32_t)map->render_pos[1] + y * map->cell_render_size,
						}, 1.0);
				}
			}
		}
	}
}

static void		map_minimap_player_render(t_wolf3d_map *map,
					t_framebuffer *framebuffer, t_player *player)
{
	t_surface	player_image;

	player_image.h = 4;
	player_image.w = 4;
	player_image.pixels = (uint32_t[16]){
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff,
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff,
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff,
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff};
	if (player->grid_pos[0] >= 0 && player->grid_pos[0] < map->size &&
		player->grid_pos[1] >= 0 && player->grid_pos[1] < map->size)
		l3d_image_place(
			&(t_surface){.h = framebuffer->height,
			.w = framebuffer->width, .pixels = framebuffer->buffer},
				&player_image,
			(int32_t[2]){
				(int32_t)map->render_pos[0] +
					player->grid_pos[0] * map->cell_render_size,
				(int32_t)map->render_pos[1] +
					player->grid_pos[1] * map->cell_render_size,
			}, 1.0);
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
				image = hash_map_get(map->map_images, c_floor);
				if (image != NULL)
					l3d_image_place(
					&(t_surface){.h = framebuffer->height,
					.w = framebuffer->width, .pixels = framebuffer->buffer},
						image,
					(int32_t[2]){
						(int32_t)map->render_pos[0] + x * map->cell_render_size,
						(int32_t)map->render_pos[1] + y * map->cell_render_size,
					}, 1.0);
			}
		}
	}
	map_minimap_player_render(map, framebuffer, player);
}

static void		map_minimap_partial_player_render(t_wolf3d_map *map,
					t_framebuffer *framebuffer, float minimap_size,
					t_player *player)
{
	t_surface	player_image;

	player_image.h = 4;
	player_image.w = 4;
	player_image.pixels = (uint32_t[16]){
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff,
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff,
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff,
		0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff};
	if (player->grid_pos[0] >= 0 && player->grid_pos[0] < map->size &&
		player->grid_pos[1] >= 0 && player->grid_pos[1] < map->size)
		l3d_image_place(
			&(t_surface){.h = framebuffer->height,
			.w = framebuffer->width, .pixels = framebuffer->buffer},
				&player_image,
			(int32_t[2]){
				(int32_t)map->render_pos[0] +
					player->grid_pos[0] * map->cell_render_size
					- player->grid_pos[0] * map->cell_render_size
					+ minimap_size / 2.0,
				(int32_t)map->render_pos[1] +
					player->grid_pos[1] * map->cell_render_size
					- player->grid_pos[1] * map->cell_render_size
					+ minimap_size / 2.0,
			}, 1.0);
}

void			map_minimap_render_partial(t_wolf3d_map *map,
					t_framebuffer *framebuffer,
					float minimap_size,
					t_player *player)
{
	int32_t		x;
	int32_t		y;
	t_surface	*image;
	uint32_t	minimap_pixels[(int32_t)minimap_size * (int32_t)minimap_size];
	t_surface	minimap;

	minimap.w = (uint32_t)minimap_size;
	minimap.h = (uint32_t)minimap_size;
	minimap.pixels = minimap_pixels;
	y = -1;
	while (++y < (int32_t)minimap_size * (int32_t)minimap_size)
		minimap_pixels[y] = 0x000000ff;
	y = -1;
	while (++y < map->size)
	{
		x = -1;
		while (++x < map->size)
		{
			if ((map->grid[y * map->size + x] & c_floor))
			{
				image = hash_map_get(map->map_images, c_floor);
				if (image != NULL)
					l3d_image_place(&minimap, image,
					(int32_t[2]){x * map->cell_render_size
							- player->grid_pos[0] * map->cell_render_size
							+ minimap_size / 2.0,
						y * map->cell_render_size
							- player->grid_pos[1] * map->cell_render_size
							+ minimap_size / 2.0,
					}, 1.0);
			}
		}
	}
	l3d_image_place(&(t_surface){.h = framebuffer->height,
			.w = framebuffer->width, .pixels = framebuffer->buffer},
			&minimap,
		(int32_t[2]){map->render_pos[0], map->render_pos[1]}, 0.5);
	map_minimap_partial_player_render(map, framebuffer,
		minimap_size, player);
}

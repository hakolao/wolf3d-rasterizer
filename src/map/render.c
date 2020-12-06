/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 23:51:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			map_boundary_render(t_wolf3d_map *map,
					t_framebuffer *framebuffer,
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

void			map_features_render(t_wolf3d_map *map,
					t_framebuffer *framebuffer)
{
	int32_t		x;
	int32_t		y;
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
				if ((map->grid[y * map->size + x] & 1 << i))
					if ((image = hash_map_get(map->map_images, 1 << i)))
						l3d_image_place(&(t_surface){.h = framebuffer->height,
						.w = framebuffer->width, .pixels = framebuffer->buffer},
						image, (int32_t[2]){(int32_t)map->render_pos[0] +
							x * map->cell_render_size,
						(int32_t)map->render_pos[1] + y * map->cell_render_size,
						}, 1.0);
			}
		}
	}
}

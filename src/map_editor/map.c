/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:56:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/20 01:25:39 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		rescale_image_assets(t_map_editor *app)
{
	float		size;
	int32_t		i;
	t_surface	*curr_image;
	uint32_t	key;

	size = (int32_t)app->map->render_size / app->map->size + 1;
	i = -1;
	while (++i < (int32_t)sizeof(uint32_t) * 4)
	{
		key = 1 << i;
		if ((curr_image = hash_map_get(app->map_images, key)))
		{
			hash_map_add(app->map_images, key,
				l3d_image_scaled(curr_image, size, size));
			free(curr_image->pixels);
			free(curr_image);
		}
	}
}

void			init_image_assets(t_map_editor *app)
{
	error_check(!(app->map_images = hash_map_create(10)),
		"Failed to create hash table");
	hash_map_add(app->map_images, c_floor,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/floor.bmp"));
	hash_map_add(app->map_images, c_floor_start,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/floor_start.bmp"));
	hash_map_add(app->map_images, c_wall_up,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_up.bmp"));
	hash_map_add(app->map_images, c_wall_right,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_right.bmp"));
	hash_map_add(app->map_images, c_wall_down,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_down.bmp"));
	hash_map_add(app->map_images, c_wall_left,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/wall_left.bmp"));
	hash_map_add(app->map_images, c_block_nw,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_nw.bmp"));
	hash_map_add(app->map_images, c_block_ne,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_ne.bmp"));
	hash_map_add(app->map_images, c_block_se,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_se.bmp"));
	hash_map_add(app->map_images, c_block_sw,
		l3d_read_bmp_image_32bit_rgba_surface("assets/map_editor/block_sw.bmp"));
}

void			rescale_map(t_map_editor *app)
{
	app->map->render_size = app->window->height * 0.8;
	app->map->cell_render_size =
		(float)app->map->render_size / (float)app->map->size;
	ml_vector2_copy((t_vec2){app->window->width / 2 - app->map->render_size / 2,
			app->window->height / 2 - app->map->render_size / 2},
		app->grid_pos);
	rescale_image_assets(app);
}

void			init_map(t_map_editor *app, int size)
{
	t_file_contents	*file;
	char			header[4];

	error_check(!(app->map = malloc(sizeof(t_wolf3d_map))),
		"Failed to malloc map");
	if (app->filename == NULL || (!(file = read_file(app->filename))))
	{
		error_check(!(app->map->grid = malloc(sizeof(uint32_t) * size * size)),
			"Failed to malloc map grid");
		ft_memset(app->map->grid, 0, sizeof(uint32_t) * size * size);
		app->map->size = size;
	}
	else
	{
		ft_memcpy(&header, file->buf, 4);
		if (!ft_strequ(header, "MAP\0"))
			error_check(true,
			"Invalid file, not a map file. First 4 bytes must be MAP\0");
		ft_memcpy(&app->map->size, file->buf + 4, 4);
		error_check(!(app->map->grid = malloc(sizeof(uint32_t) *
			app->map->size * app->map->size)),
			"Failed to malloc map grid");
		ft_memcpy(app->map->grid, file->buf + 8, file->size - 8);
		destroy_file_contents(file);
	}
	init_image_assets(app);
	rescale_map(app);
}

static void		map_grid_render(t_map_editor *app, t_vec2 pos, uint32_t color)
{
	uint32_t	*buffer;
	int32_t		i;
	float		interval;

	buffer = app->window->buffers->framebuffer;
	interval = app->map->cell_render_size;
	i = -1;
	while (++i <= app->map->size)
	{
		l3d_line_draw(buffer,
			(uint32_t[2]){app->window->width, app->window->height
		}, (int32_t[2][2]){{pos[0] + (float)(i * interval), pos[1]},
			{pos[0] + (float)(i * interval), pos[1] +
				app->map->render_size}}, color);
		l3d_line_draw(buffer,
			(uint32_t[2]){app->window->width, app->window->height
		}, (int32_t[2][2]){{pos[0], pos[1] + (float)(i * interval)},
			{pos[0] + app->map->render_size, pos[1] + (float)(i * interval)}},
			color);
	}
}

static void		map_features_render(t_map_editor *app)
{
	int32_t		x;
	int32_t		y;
	uint32_t	feature_flag;
	int32_t		i;
	t_surface	*image;

	y = -1;
	while (++y < app->map->size)
	{
		x = -1;
		while (++x < app->map->size)
		{
			i = -1;
			while (++i < 32)
			{
				feature_flag = 1 << i;
				if ((app->map->grid[y * app->map->size + x] & feature_flag))
				{
					image = hash_map_get(app->map_images, feature_flag);
					if (image != NULL)
						l3d_framebuffer_image_place(
							&(t_surface){.h = app->window->height,
								.w = app->window->width,
							.pixels = app->window->buffers->framebuffer},
							image,
							(int32_t[2]){
								(int32_t)app->grid_pos[0] + x * app->map->cell_render_size,
								(int32_t)app->grid_pos[1] + y * app->map->cell_render_size,
							}, 1.0);
				}
			}
		}
	}
}

void			map_render(t_map_editor *app, t_vec2 pos)
{
	uint32_t	color;

	color = 0xFFFFFFFF;
	map_features_render(app);
	map_grid_render(app, pos, color);
}

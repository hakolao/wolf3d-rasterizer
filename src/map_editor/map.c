/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:56:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/11 17:33:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		rescale_image_assets(t_map_editor *app)
{
	float		size;
	int32_t		i;
	t_surface	*curr_image;
	t_surface	*scaled_image;

	size = (float)app->map->render_size / (float)app->map->size;
	i = -1;
	while (++i < app->num_images)
	{
		curr_image = hash_map_get(app->map_images, app->image_keys[i]);
		scaled_image = l3d_image_scaled(curr_image, size, size);
		free(curr_image->pixels);
		free(curr_image);
		hash_map_add(app->map_images, app->image_keys[i], scaled_image);
	}
}

void			init_image_assets(t_map_editor *app)
{
	t_surface		**surfaces;
	int32_t			i;

	app->num_images = 6;
	error_check(!(surfaces = malloc(sizeof(t_surface*) * app->num_images)),
		"Failed to malloc surfaces pointer array");
	error_check(!(app->map_images = hash_map_create(app->num_images)),
		"Failed to create hash table");
	app->image_keys[0] = c_floor;
	app->image_keys[1] = c_floor_start;
	app->image_keys[2] = c_wall_up;
	app->image_keys[3] = c_wall_right;
	app->image_keys[4] = c_wall_down;
	app->image_keys[5] = c_wall_left;
	i = -1;
	while (++i < app->num_images)
	{
		error_check(!(surfaces[i] = malloc(sizeof(t_surface))),
		"Failed to malloc surface");
		hash_map_add(app->map_images, app->image_keys[i], surfaces[i]);
	}
	l3d_read_bmp_image_32bit_rgba("assets/map_editor/floor.bmp",
		&surfaces[0]->pixels, &surfaces[0]->w, &surfaces[0]->h);
	l3d_read_bmp_image_32bit_rgba("assets/map_editor/floor_start.bmp",
		&surfaces[1]->pixels, &surfaces[1]->w, &surfaces[1]->h);
	l3d_read_bmp_image_32bit_rgba("assets/map_editor/wall_up.bmp",
		&surfaces[2]->pixels, &surfaces[2]->w, &surfaces[2]->h);
	l3d_read_bmp_image_32bit_rgba("assets/map_editor/wall_right.bmp",
		&surfaces[3]->pixels, &surfaces[3]->w, &surfaces[3]->h);
	l3d_read_bmp_image_32bit_rgba("assets/map_editor/wall_down.bmp",
		&surfaces[4]->pixels, &surfaces[4]->w, &surfaces[4]->h);
	l3d_read_bmp_image_32bit_rgba("assets/map_editor/wall_left.bmp",
		&surfaces[5]->pixels, &surfaces[5]->w, &surfaces[5]->h);
	free(surfaces);
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
	error_check(!(app->map = malloc(sizeof(t_wolf3d_map))),
		"Failed to malloc map");
	error_check(!(app->map->grid = malloc(sizeof(uint32_t) * size * size)),
		"Failed to malloc map grid");
	ft_memset(app->map, 0, sizeof(uint32_t));
	app->map->size = size;
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

void			map_render(t_map_editor *app, t_vec2 pos)
{
	uint32_t	color;

	color = 0xFFFFFFFF;
	map_grid_render(app, pos, color);
}
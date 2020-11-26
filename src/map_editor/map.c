/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:56:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/26 14:25:59 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			map_editor_map_init(t_map_editor *app, int size)
{
	t_file_contents	*file;
	float			render_size;
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
	map_init_image_assets(&app->map->map_images);
	render_size = app->window->height * 0.8;
	map_render_resize(app->map, render_size,
		(t_vec2){app->window->width / 2 - render_size / 2,
			app->window->height / 2 - render_size / 2});
}

void			map_editor_map_render(t_map_editor *app, t_vec2 pos)
{
	map_features_render(app->map, app->window->framebuffer);
	map_grid_render(app->map, app->window->framebuffer, pos, 0xFFFFFFFF);
}


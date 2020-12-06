/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:12:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			fill_existing_map_file_data(t_map_editor *app,
					t_file_contents *file)
{
	char			header[4];

	ft_memcpy(&header, file->buf, 4);
	if (!ft_strequ(header, "MAP\0"))
		error_check(true,
		"Invalid file, not a map file. First 4 bytes must be MAP\0");
	ft_memcpy(&app->map->size, file->buf + 4, 4);
	error_check(!(app->map->grid = malloc(sizeof(uint32_t) *
		app->map->size * app->map->size)),
		"Failed to malloc map grid");
	ft_memcpy(app->map->grid, file->buf + 8, file->size - 8);
}

void			map_editor_map_init(t_map_editor *app, int size)
{
	t_file_contents	*file;
	float			render_size;

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
		fill_existing_map_file_data(app, file);
		destroy_file_contents(file);
	}
	map_init_image_assets(&app->map->map_images);
	render_size = app->window->height * 0.8;
	map_render_resize(app->map, render_size,
		(t_vec2){app->window->width / 2 - render_size / 2,
			app->window->height / 2 - render_size / 2});
}

void			map_editor_map_render(t_map_editor *app)
{
	map_features_render(app->map, app->window->framebuffer);
	map_boundary_render(app->map, app->window->framebuffer, 0xFFFFFFFF);
}

void			update_mouse_grid_pos(t_map_editor *app)
{
	ml_vector2_copy((t_vec2){
		ft_floor(((float)(app->mouse.x - app->map->render_pos[0]) /
			app->map->cell_render_size)),
		ft_floor(((float)(app->mouse.y - app->map->render_pos[1]) /
			app->map->cell_render_size))},
		app->mouse_grid_pos);
}

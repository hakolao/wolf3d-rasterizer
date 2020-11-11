/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:56:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/11 15:10:52 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			init_map(t_map_editor *app, int size)
{
	error_check(!(app->map = malloc(sizeof(uint32_t) * size * size)),
		"Failed ot malloc map");
	ft_memset(app->map, 0, sizeof(uint32_t));
}

static void		map_grid_render(t_map_editor *app, t_vec2 pos)
{
	int32_t		grid_render_size;
	uint32_t	*buffer;

	grid_render_size = app->window->height * 0.8;
	buffer = app->window->buffers->framebuffer;
	l3d_line_draw(buffer, (uint32_t[2]){ app->window->width, app->window->height
	}, (int32_t[2][2]){{pos[0], pos[1]}, {pos[0], pos[1] + grid_render_size}},
		0xFFFFFFFF);
	l3d_line_draw(buffer, (uint32_t[2]){ app->window->width, app->window->height
	}, (int32_t[2][2]){{pos[0], pos[1]}, {pos[0] + grid_render_size, pos[1]}},
		0xFFFFFFFF);
	l3d_line_draw(buffer, (uint32_t[2]){ app->window->width, app->window->height
	}, (int32_t[2][2]){{pos[0] + grid_render_size, pos[1]},
		{pos[0] + grid_render_size, pos[1] + grid_render_size}}, 0xFFFFFFFF);
	l3d_line_draw(buffer, (uint32_t[2]){ app->window->width, app->window->height
	}, (int32_t[2][2]){{pos[0], pos[1] + grid_render_size},
		{pos[0] + grid_render_size, pos[1] + grid_render_size}}, 0xFFFFFFFF);
	app->map->render_size = grid_render_size;
}

void			map_render(t_map_editor *app, t_vec2 pos)
{
	map_grid_render(app, pos);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 20:49:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int32_t	room_at(t_map_editor *app, int32_t x, int32_t y)
{
	int32_t		cell_index;

	if ((y < 0 || y >= app->map->size || x < 0 || x >= app->map->size))
		return (0);
	cell_index = y * app->map->size + x;
	return ((app->map->grid[cell_index] & m_room));
}

static void		get_neighbor_rooms(t_map_editor *app, int32_t xy[2],
								int32_t neighbors[9], uint32_t *cell)
{
	neighbors[0] = room_at(app, xy[0] - 1, xy[1] - 1);
	neighbors[1] = room_at(app, xy[0], xy[1] - 1);
	neighbors[2] = room_at(app, xy[0] + 1, xy[1] - 1);
	neighbors[3] = room_at(app, xy[0] - 1, xy[1]);
	neighbors[4] = (*cell & m_room);
	neighbors[5] = room_at(app, xy[0] + 1, xy[1]);
	neighbors[6] = room_at(app, xy[0] - 1, xy[1] + 1);
	neighbors[7] = room_at(app, xy[0], xy[1] + 1);
	neighbors[8] = room_at(app, xy[0] + 1, xy[1] + 1);
}

static void		update_ceiling(uint32_t *cell)
{
	float		window_p;

	window_p = l3d_rand_d();
	if (window_p < 0.2)
	{
		*cell ^= (c_ceiling & *cell);
		*cell |= c_ceiling_window;
	}
	else
	{
		*cell ^= (c_ceiling_window & *cell);
		*cell |= c_ceiling;
	}
}

void			update_map_cell_features(t_map_editor *app)
{
	int32_t		y;
	int32_t		x;
	uint32_t	*cell;
	int32_t		rooms[9];

	y = -1;
	while (++y < app->map->size)
	{
		x = -1;
		while (++x < app->map->size)
		{
			cell = app->map->grid + y * app->map->size + x;
			if (!(*cell & m_room))
				continue ;
			get_neighbor_rooms(app, (int32_t[2]){x, y}, rooms, cell);
			*cell ^= (p_all & *cell);
			if (modify_dead_end_pattern(cell,
				rooms) || modify_corner_pattern(cell,
				rooms) || modify_corridor_pattern(cell,
				rooms) || modify_wall_pattern(cell,
				rooms) || modify_surround_pattern(cell, rooms))
				;
			update_ceiling(cell);
		}
	}
}

void			handle_feature_placement(t_map_editor *app)
{
	int32_t		i;
	uint32_t	*cell;

	if (app->mouse_grid_pos[0] < 0 ||
		app->mouse_grid_pos[0] >= app->map->size ||
		app->mouse_grid_pos[1] < 0 ||
		app->mouse_grid_pos[1] >= app->map->size)
		return ;
	cell = &app->map->grid[(int32_t)app->mouse_grid_pos[1] *
		app->map->size + (int32_t)app->mouse_grid_pos[0]];
	if (app->selected_feature == m_start)
	{
		i = -1;
		while (++i < app->map->size * app->map->size)
			if (app->map->grid[i] & m_start)
				app->map->grid[i] ^= m_start;
		*cell |= m_room;
		*cell |= m_start;
	}
	else if (app->selected_feature == m_clear)
		*cell = m_clear;
	else if (app->selected_feature == m_room)
		*cell |= m_room;
	update_map_cell_features(app);
}

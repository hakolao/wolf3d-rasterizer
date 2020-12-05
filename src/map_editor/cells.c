/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 13:57:03 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/05 17:27:29 by ohakola+vei      ###   ########.fr       */
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

static void		get_neighbor_rooms(t_map_editor *app, int32_t x, int32_t y,
								int32_t neighbors[9], uint32_t *cell)
{
	neighbors[0] = room_at(app, x - 1, y - 1);
	neighbors[1] = room_at(app, x, y - 1);
	neighbors[2] = room_at(app, x + 1, y - 1);
	neighbors[3] = room_at(app, x - 1, y);
	neighbors[4] = (*cell & m_room);
	neighbors[5] = room_at(app, x + 1, y);
	neighbors[6] = room_at(app, x - 1, y + 1);
	neighbors[7] = room_at(app, x, y + 1);
	neighbors[8] = room_at(app, x + 1, y + 1);
}

static void		update_ceiling(uint32_t *cell)
{
	float		ceiling_p;

	ceiling_p = l3d_rand_d();
	if (ceiling_p < 0.1)
		*cell |= p_ceiling;
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
			get_neighbor_rooms(app, x, y, rooms, cell);
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

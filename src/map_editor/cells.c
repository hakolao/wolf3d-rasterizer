/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 13:57:03 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/17 15:42:49 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_bool	room_at(t_map_editor *app, int32_t x, int32_t y)
{
	int32_t		cell_index;

	if ((y < 0 || y >= app->map->size || x < 0 || x >= app->map->size))
		return (false);
	cell_index = y * app->map->size + x;
	return ((app->map->grid[cell_index] & m_room));
}

static void		get_neighbor_rooms(t_map_editor *app, int32_t x, int32_t y,
								t_bool neighbors[9], uint32_t *cell)
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

t_bool			rooms_equal_pattern(t_bool rooms[9], t_bool pattern[9])
{
	int32_t	i;

	i = -1;
	while (++i < 9)
		if (rooms[i] != pattern[i])
			return (false);
	return (true);
}

void			update_map_cell_features(t_map_editor *app)
{
	int32_t		y;
	int32_t		x;
	uint32_t	*cell;
	t_bool		neighbors[9];

	ft_printf("Update map cell features\n");
	y = -1;
	while (++y < app->map->size)
	{
		x = -1;
		while (++x < app->map->size)
		{
			cell = app->map->grid + y * app->map->size + x;
			if (!(*cell & m_room))
				continue ;
			get_neighbor_rooms(app, x, y, neighbors, cell);
			*cell ^= (p_dead_all & *cell); // Just removes all room building cell feature bits
			ft_printf("%d %d %d\n%d %d %d\n%d %d %d\n--\n",
				neighbors[0], neighbors[1], neighbors[2],
				neighbors[3], neighbors[4], neighbors[5],
				neighbors[6], neighbors[7], neighbors[8]);
			if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
															0, 1, 0,
															0, 0, 0}) ||
				rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 0,
															0, 1, 0,
															0, 0, 0}) ||
				rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
															0, 1, 0,
															0, 0, 0}) ||
				rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
															0, 1, 0,
															0, 0, 1}) ||
				rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
															0, 1, 0,
															1, 0, 0}))
			{
				*cell |= p_dead_all;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
															1, 1, 1,
															1, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
																1, 1, 1,
																0, 1, 0}))
			{
				*cell |= p_middle_floor;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																1, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 1,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																1, 1, 1,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 1,
																1, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 0,
																1, 1, 1,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 1,
																1, 1, 1,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																1, 1, 1,
																1, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 1,
																1, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																1, 1, 1,
																1, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 1,
																1, 1, 1,
																1, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 0,
																1, 1, 1,
																1, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 1,
																1, 1, 1,
																1, 0, 1}))
			{
				*cell |= p_corr_horz;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
																0, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																0, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																0, 1, 0,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 0,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 0,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																0, 1, 0,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 0,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 0,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																0, 1, 0,
																1, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 0,
																1, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 0,
																1, 1, 1}))
			{
				*cell |= p_corr_vert;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
															1, 1, 0,
															0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 0,
																1, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 0,
																1, 1, 0,
																0, 0, 0}))
			{
				*cell |= p_dead_right;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
															0, 1, 0,
															0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																0, 1, 0,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 0,
																0, 0, 0}))
			{
				*cell |= p_dead_down;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
															0, 1, 1,
															0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 1,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																0, 1, 1,
																0, 0, 0}))
			{
				*cell |= p_dead_left;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 0,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 0,
																0, 1, 1}))
			{
				*cell |= p_dead_up;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
																1, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																1, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																1, 1, 0,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																1, 1, 0,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																1, 1, 0,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																1, 1, 0,
																1, 1, 0}))
			{
				*cell |= p_wall_right;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
																1, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																1, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																1, 1, 1,
																1, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																1, 1, 1,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																1, 1, 1,
																1, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																1, 1, 1,
																0, 0, 0}))
			{
				*cell |= p_wall_down;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
															0, 1, 1,
															0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 1,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 1,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 1,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 1,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 1,
																0, 1, 1}))
			{
				*cell |= p_wall_left;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 1,
																0, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 1,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 0,
																1, 1, 1,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																1, 1, 1,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 1,
																1, 1, 1,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 1,
																1, 1, 1}))
			{
				*cell |= p_wall_up;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																1, 1, 0,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																1, 1, 0,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 0,
																1, 1, 0,
																0, 0, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
																1, 1, 0,
																0, 0, 0}))
			{
				*cell |= p_corner_se;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 1, 1,
																0, 1, 1,
																0, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 1,
																0, 1, 1,
																1, 0, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 1, 0,
																0, 1, 1,
																0, 0, 0}))
			{
				*cell |= p_corner_sw;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 1,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 1,
																1, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){1, 0, 0,
																0, 1, 1,
																0, 1, 1}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																0, 1, 1,
																0, 1, 0}))
			{
				*cell |= p_corner_nw;
			}
			else if (rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 0,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 0,
																1, 1, 1}) || 
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 1,
																1, 1, 0,
																1, 1, 0}) ||
					rooms_equal_pattern(neighbors, (t_bool[9]){0, 0, 0,
																1, 1, 0,
																0, 1, 0}))
			{
				*cell |= p_corner_ne;
			}
		}
	}
}
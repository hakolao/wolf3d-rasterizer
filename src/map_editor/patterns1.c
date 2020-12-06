/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 01:13:37 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:24:11 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_bool	modify_3_corner_blocks(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 0, 1, 0}))
	{
		*cell |= c_block_nw | c_block_se | c_block_sw;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 0, 1, 1}))
	{
		*cell |= c_block_ne | c_block_nw | c_block_sw;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 1, 1, 0}))
	{
		*cell |= c_block_ne | c_block_nw | c_block_se;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 0, 1, 0}))
	{
		*cell |= c_block_ne | c_block_sw | c_block_se;
		return (true);
	}
	return (false);
}

static t_bool	modify_2_corner_blocks(uint32_t *cell, int32_t rooms[9])
{
	uint32_t	blocks;

	blocks = 0;
	if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 1, 1, 1, 1, 0, 1, 0}))
		blocks |= c_block_sw | c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 1, 1, 1}))
		blocks |= c_block_nw | c_block_ne;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 1, 1, 0}))
		blocks |= c_block_ne | c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 0, 1, 1}))
		blocks |= c_block_nw | c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 0, 1, 1}))
		blocks |= c_block_sw | c_block_ne;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 1, 1, 0}))
		blocks |= c_block_nw | c_block_se;
	if (blocks != 0)
	{
		*cell |= blocks;
		return (true);
	}
	return (false);
}

static t_bool	modify_middle_floor_corner_blocks(uint32_t *cell,
					int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 0, 1, 0}))
	{
		*cell |= c_block_ne | c_block_nw | c_block_se | c_block_sw;
		return (true);
	}
	if (modify_3_corner_blocks(cell, rooms))
		return (true);
	else if (modify_2_corner_blocks(cell, rooms))
		return (true);
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 1, 1, 1, 1, 0, 1, 1}))
		*cell |= c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 1, 1, 1, 1, 1, 1, 0}))
		*cell |= c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 1, 1, 1}))
		*cell |= c_block_ne;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 1, 1, 1}))
		*cell |= c_block_nw;
	return (true);
}

t_bool			modify_surround_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 0, 1, 0, 2, 0, 2}))
	{
		*cell |= p_dead_all;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 1, 1, 1, 2, 1, 2}))
	{
		*cell |= p_middle_floor;
		modify_middle_floor_corner_blocks(cell, rooms);
		return (true);
	}
	return (false);
}

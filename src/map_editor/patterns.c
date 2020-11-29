/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:30:56 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/18 18:16:47 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_bool	rooms_eq_pattern(int32_t rooms[9], int32_t pattern[9])
{
	int32_t	i;

	i = -1;
	while (++i < 9)
		if (rooms[i] != pattern[i] && pattern[i] != OPTIONAL_PATTERN)
			return (false);
	return (true);
}

t_bool			modify_dead_end_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 0, 2, 0, 2}))
	{
		*cell |= p_dead_right;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 0, 1, 0, 2, 0, 2}))
	{
		*cell |= p_dead_down;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 0, 1, 1, 2, 0, 2}))
	{
		*cell |= p_dead_left;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 0, 1, 0, 2, 1, 2}))
	{
		*cell |= p_dead_up;
		return (true);
	}
	return (false);
}

t_bool			modify_corner_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms,
		(int32_t[9]){2, 1, 2, 1, 1, 0, 2, 0, 2}))
	{
		*cell |= p_corner_se;
		if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 2, 1, 1, 0, 2, 0, 2}))
			*cell |= c_block_nw;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 0, 1, 1, 2, 0, 2}))
	{
		*cell |= p_corner_sw;
		if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 0, 0, 1, 1, 2, 0, 2}))
			*cell |= c_block_ne;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 0, 1, 1, 2, 1, 2}))
	{
		*cell |= p_corner_nw;
		if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 0, 1, 1, 2, 1, 0}))
			*cell |= c_block_se;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 0, 2, 1, 2}))
	{
		*cell |= p_corner_ne;
		if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 0, 0, 1, 2}))
			*cell |= c_block_sw;
		return (true);
	}
	return (false);
}

t_bool			modify_corridor_pattern(uint32_t *cell, int32_t rooms[9])
{
 	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 1, 2, 0, 2}))
	{
		*cell |= p_corr_horz;
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 0, 1, 0, 2, 1, 2}))
	{
		*cell |= p_corr_vert;
		return (true);
	}
	return (false);
}

static t_bool	modify_wall_right_blocks(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 2, 1, 1, 0, 0, 1, 2}))
		*cell |= c_block_sw | c_block_nw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 2, 1, 1, 0, 1, 1, 2}))
		*cell |= c_block_nw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 2, 1, 1, 0, 0, 1, 2}))
		*cell |= c_block_sw;
	return (true);
}

static t_bool	modify_wall_down_blocks(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 2, 0, 2}))
		*cell |= c_block_ne | c_block_nw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 2, 0, 2}))
		*cell |= c_block_nw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 2, 0, 2}))
		*cell |= c_block_ne;
	return (true);
}

static t_bool	modify_wall_left_blocks(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 0, 0, 1, 1, 2, 1, 0}))
		*cell |= c_block_se | c_block_ne;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 1, 0, 1, 1, 2, 1, 0}))
		*cell |= c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 0, 0, 1, 1, 2, 1, 1}))
		*cell |= c_block_ne;
	return (true);
}

static t_bool	modify_wall_up_blocks(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 1, 0, 1, 0}))
		*cell |= c_block_se | c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 1, 1, 1, 0}))
		*cell |= c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 1, 0, 1, 1}))
		*cell |= c_block_sw;
	return (true);
}

t_bool			modify_wall_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 1, 1, 0, 2, 1, 2}))
	{
		*cell |= p_wall_right;
		modify_wall_right_blocks(cell, rooms);
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 1, 1, 1, 2, 0, 2}))
	{
		*cell |= p_wall_down;
		modify_wall_down_blocks(cell, rooms);
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 0, 1, 1, 2, 1, 2}))
	{
		*cell |= p_wall_left;
		modify_wall_left_blocks(cell, rooms);
		return (true);
	}
	else if (rooms_eq_pattern(rooms, (int32_t[9]){2, 0, 2, 1, 1, 1, 2, 1, 2}))
	{
		*cell |= p_wall_up;
		modify_wall_up_blocks(cell, rooms);
		return (true);
	}
	return (false);
}

static t_bool	modify_middle_floor_corner_blocks(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 0, 1, 0}))
		*cell |= c_block_ne | c_block_nw | c_block_se | c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 0, 1, 0}))
		*cell |= c_block_nw | c_block_se | c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 0, 1, 1}))
		*cell |= c_block_ne | c_block_nw | c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 1, 1, 0}))
		*cell |= c_block_ne | c_block_nw | c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 0, 1, 0}))
		*cell |= c_block_ne | c_block_sw | c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 1, 1, 1, 1, 0, 1, 0}))
		*cell |= c_block_sw | c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 0, 1, 1, 1, 1, 1, 1}))
		*cell |= c_block_nw | c_block_ne;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 1, 1, 0}))
		*cell |= c_block_ne | c_block_se;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 0, 1, 1}))
		*cell |= c_block_nw | c_block_sw;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){1, 1, 0, 1, 1, 1, 0, 1, 1}))
		*cell |= c_block_sw | c_block_ne;
	else if (rooms_eq_pattern(rooms, (int32_t[9]){0, 1, 1, 1, 1, 1, 1, 1, 0}))
		*cell |= c_block_nw | c_block_se;
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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 01:13:50 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:24:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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

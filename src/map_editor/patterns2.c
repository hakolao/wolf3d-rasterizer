/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:26:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_bool			rooms_eq_pattern(int32_t rooms[9], int32_t pattern[9])
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

static t_bool	modify_corner_pattern_partial(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_eq_pattern(rooms, (int32_t[9]){2, 1, 2, 1, 1, 0, 2, 0, 2}))
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
	return (false);
}

t_bool			modify_corner_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (modify_corner_pattern_partial(cell, rooms))
		return (true);
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

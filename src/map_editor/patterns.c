/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:30:56 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/17 16:33:56 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_bool	rooms_equal_pattern(int32_t rooms[9], int32_t pattern[9])
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
	if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 1, 1, 0, 2, 0, 2}))
	{
		*cell |= p_dead_right;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 0, 1, 0, 2, 0, 2}))
	{
		*cell |= p_dead_down;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 0, 1, 1, 2, 0, 2}))
	{
		*cell |= p_dead_left;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 0, 1, 0, 2, 1, 2}))
	{
		*cell |= p_dead_up;
		return (true);
	}
	return (false);
}

t_bool			modify_corner_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 1, 1, 0, 2, 0, 2}))
	{
		*cell |= p_corner_se;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 0, 1, 1, 2, 0, 2}))
	{
		*cell |= p_corner_sw;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 0, 1, 1, 2, 1, 2}))
	{
		*cell |= p_corner_nw;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 1, 1, 0, 2, 1, 2}))
	{
		*cell |= p_corner_ne;
		return (true);
	}
	return (false);
}

t_bool			modify_corridor_pattern(uint32_t *cell, int32_t rooms[9])
{
 	if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 1, 1, 1, 2, 0, 2}))
	{
		*cell |= p_corr_horz;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 0, 1, 0, 2, 1, 2}))
	{
		*cell |= p_corr_vert;
		return (true);
	}
	return (false);
}

t_bool			modify_wall_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 	1, 1, 0, 2, 1, 2}))
	{
		*cell |= p_wall_right;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 1, 1, 1, 2, 0, 2}))
	{
		*cell |= p_wall_down;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 0, 1, 1, 2, 1, 2}))
	{
		*cell |= p_wall_left;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 1, 1, 1, 2, 1, 2}))
	{
		*cell |= p_wall_up;
		return (true);
	}
	return (false);
}

t_bool			modify_surround_pattern(uint32_t *cell, int32_t rooms[9])
{
	if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 0, 2, 0, 1, 0, 2, 0, 2}))
	{
		*cell |= p_dead_all;
		return (true);
	}
	else if (rooms_equal_pattern(rooms,
		(int32_t[9]){2, 1, 2, 1, 1, 1, 2, 1, 2}))
	{
		*cell |= p_middle_floor;
		return (true);
	}
	return (false);
}

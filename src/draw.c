/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:15:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		draw_frame(t_wolf3d *app)
{
	(void)app;
}

/*
**	Converts pixel position from screen coordinates to frame buffer index
*/

int			screen_to_frame_coords(int x, int y)
{
	return (y * WIDTH + x);
}
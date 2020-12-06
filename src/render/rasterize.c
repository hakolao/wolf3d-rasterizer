/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 23:25:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** If all points are on the same side outside screen bounds, don't render
*/

static t_bool	triangle_outside_render_area(t_triangle *triangle,
					t_sub_framebuffer *sub_buffer)
{
	t_vec2		xy1;
	t_vec2		xy2;
	t_vec2		xy3;

	ml_vector2_copy(triangle->points_2d[0], xy1);
	ml_vector2_copy(triangle->points_2d[1], xy2);
	ml_vector2_copy(triangle->points_2d[2], xy3);
	return ((xy1[0] + sub_buffer->x_offset < 0.0 &&
			xy2[0] + sub_buffer->x_offset < 0.0 &&
			xy3[0] + sub_buffer->x_offset < 0.0) ||
			(xy1[0] + sub_buffer->x_offset >= sub_buffer->width &&
			xy2[0] + sub_buffer->x_offset >= sub_buffer->width &&
			xy3[0] + sub_buffer->x_offset >= sub_buffer->width) ||
			(xy1[1] + sub_buffer->y_offset < 0.0 &&
			xy2[1] + sub_buffer->y_offset < 0.0 &&
			xy3[1] + sub_buffer->y_offset < 0.0) ||
			(xy1[1] + sub_buffer->y_offset >= sub_buffer->height &&
			xy2[1] + sub_buffer->y_offset >= sub_buffer->height &&
			xy3[1] + sub_buffer->y_offset >= sub_buffer->height));
}

/*
** Thread loops through renderable triangles and
** finally filters out those that don't fit sub_buffer's limits
*/

void			rasterize_triangles(t_render_work *work)
{
	t_sub_framebuffer	*sub_buffer;
	t_triangle			*triangle;
	int32_t				i;

	sub_buffer = work->sub_buffer;
	i = -1;
	while (++i < (int32_t)work->render_triangles->size)
	{
		triangle = work->render_triangles->triangles[i];
		if (triangle_outside_render_area(triangle, sub_buffer))
			continue ;
		l3d_triangle_raster(sub_buffer, triangle);
	}
}


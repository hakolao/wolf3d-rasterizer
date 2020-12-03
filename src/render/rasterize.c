/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rasterize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:38:53 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/04 00:07:35 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** Sets z value for triangle vertices. Unit size division is used to bring
** the number to a more reasonable range - to be more standard for rasterizer
** and e.g. depth shading.
*/

static void		update_triangle_vertex_zvalues(t_triangle *triangle,
					float unit_size)
{
	triangle->vtc_zvalue[0] = 1.0 /
		(triangle->vtc[0]->pos[2] / unit_size + L3D_EPSILON);
	triangle->vtc_zvalue[1] = 1.0 /
		(triangle->vtc[1]->pos[2] / unit_size + L3D_EPSILON);
	triangle->vtc_zvalue[2] = 1.0 /
		(triangle->vtc[2]->pos[2] / unit_size + L3D_EPSILON);
}

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

static void		rasterize_triangle(t_sub_framebuffer *sub_buffer,
							t_triangle *triangle)
{
	if (triangle_outside_render_area(triangle, sub_buffer))
		return ;
	l3d_triangle_raster(sub_buffer, triangle);
}

void			render_triangle(t_wolf3d *app, t_sub_framebuffer *sub_buffer,
									t_triangle *triangle)
{
	t_triangle	clipped_triangles[2];
	t_vertex	vtc[9];
	int32_t		test_clip;

	l3d_set_clipped_triangles(vtc, triangle, clipped_triangles);
	test_clip = l3d_clip_triangle(triangle,
		&app->active_scene->main_camera->viewplanes[0], clipped_triangles);
	if (test_clip == 2)
	{
		l3d_triangle_update(&clipped_triangles[0]);
		l3d_triangle_update(&clipped_triangles[1]);
		screen_intersection(app, &clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[1]);
		update_triangle_vertex_zvalues(&clipped_triangles[0], app->unit_size);
		update_triangle_vertex_zvalues(&clipped_triangles[1], app->unit_size);
		rasterize_triangle(sub_buffer, &clipped_triangles[0]);
		rasterize_triangle(sub_buffer, &clipped_triangles[1]);
	}
	else if (test_clip == 1)
	{
		l3d_triangle_update(&clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[0]);
		update_triangle_vertex_zvalues(&clipped_triangles[0], app->unit_size);
		rasterize_triangle(sub_buffer, &clipped_triangles[0]);
	}
	else
	{
		screen_intersection(app, triangle);
		update_triangle_vertex_zvalues(triangle, app->unit_size);
		rasterize_triangle(sub_buffer, triangle);
	}
}

void			set_render_triangle(t_wolf3d *app,
									t_triangle *r_triangle,
									t_triangle *triangle, t_vertex *vtc)
{
	int32_t		i;

	ft_memcpy(r_triangle, triangle, sizeof(t_triangle));
	i = -1;
	while (++i < 3)
	{
		r_triangle->vtc[i] = &vtc[i];
		ml_vector3_copy(triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_translation, r_triangle->vtc[i]->pos,
			r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_rotation,
			r_triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
	}
	l3d_triangle_update(r_triangle);
	triangle->clipped = false;
}

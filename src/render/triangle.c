/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:38:53 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/28 18:21:28 by ohakola+vei      ###   ########.fr       */
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

static void		rasterize(t_sub_framebuffer *sub_buffer,
							t_triangle *triangle, t_render_pass passes)
{
	if ((passes & rpass_zbuffer))
		l3d_triangle_set_zbuffer(sub_buffer, triangle);
	if ((passes & rpass_rasterize))
		l3d_triangle_raster(sub_buffer, triangle);
}

void			render_triangle(t_wolf3d *app, t_sub_framebuffer *sub_buffer,
									t_triangle *triangle, t_render_pass passes)
{
	t_triangle	clipped_triangles[2];
	t_vertex	vtc[9];
	int32_t		test_clip;

	l3d_set_clipped_triangles(vtc, triangle, clipped_triangles);
	test_clip = l3d_clip_triangle(triangle,
		&app->active_scene->main_camera->viewplanes[0], clipped_triangles);
	if (test_clip == 2)
	{
		screen_intersection(app, &clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[1]);
		update_triangle_vertex_zvalues(&clipped_triangles[0], app->unit_size);
		update_triangle_vertex_zvalues(&clipped_triangles[1], app->unit_size);
		rasterize(sub_buffer, &clipped_triangles[0], passes);
		rasterize(sub_buffer, &clipped_triangles[1], passes);
	}
	else if (test_clip == 1)
	{
		screen_intersection(app, &clipped_triangles[0]);
		update_triangle_vertex_zvalues(&clipped_triangles[0], app->unit_size);
		rasterize(sub_buffer, &clipped_triangles[0], passes);
	}
	else
	{
		screen_intersection(app, triangle);
		update_triangle_vertex_zvalues(triangle, app->unit_size);
		rasterize(sub_buffer, triangle, passes);
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
}

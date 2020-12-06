/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_clipping.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 17:22:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 17:23:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
**	Checks if the triangle clips with a plane and gives the indices for the
**	vertices that are on the positive side of the plane if it is clipping.
**	Return value is the number of vertices on the positive side of the plane.
*/

static			t_bool is_point_behind_near(t_plane *near, t_vec3 point)
{
	t_vec3 pldir;
	t_vec3 point_from_plane;

	ml_vector3_mul(near->normal, near->d, pldir);
	ml_vector3_sub(point, pldir, point_from_plane);
	if (ml_vector3_dot(pldir, point_from_plane) < 0)
		return (true);
	return (false);
}

int				l3d_triangle_clipping_case(t_triangle *triangle, t_plane *plane,
									int *point_indices)
{
	int i;
	t_plane *near = plane;

	i = -1;
	while (++i < 3)
	{
		if (is_point_behind_near(near, triangle->vtc[i]->pos))
		{

			if (!(is_point_behind_near(near, triangle->vtc[(i + 1) % 3]->pos))
				&& !(is_point_behind_near(near,
										triangle->vtc[(i + 2) % 3]->pos)))
			{
				point_indices[0] = i;
				return (2);
			}
		}
		else
		{
			if (is_point_behind_near(near, triangle->vtc[(i + 1) % 3]->pos) &&
				is_point_behind_near(near, triangle->vtc[(i + 2) % 3]->pos))
			{
				point_indices[0] = (i + 1) % 3;
				point_indices[1] = (i + 2) % 3;
				return (1);
			}
		}
	}
	return (0);
}

/*
**	Interpolates uv coordinates for the new vertices of the clipped triangle
**	Uses the z coordinate (i = 2) to interpolate because that is the most likely
**	one to have proper values for linear interpolation. X causes glitches.
*/

static t_bool		l3d_interpolate_clipped_uv(t_triangle *triangle, int *limits,
											t_vec3 hit, t_vec2 result)
{
	int i;
	double fraction;

	fraction = 0.0;
	i = 2;
	if (fabs(triangle->vtc[limits[0]]->pos[i] -
			 triangle->vtc[limits[1]]->pos[i]) < L3D_EPSILON)
	{
		ft_printf("!this should never happen but it will if the triangle vertices"
				  "are degenerate\n");
		return (false);
	}
	fraction = fabs(hit[i] - triangle->vtc[limits[0]]->pos[i]) /
				fabs(triangle->vtc[limits[0]]->pos[i] -
					triangle->vtc[limits[1]]->pos[i]);
	result[0] = triangle->uvs[limits[0]][0] * (1 - fraction) +
				triangle->uvs[limits[1]][0] * (fraction);

	result[1] = triangle->uvs[limits[0]][1] * (1 - fraction) +
				triangle->uvs[limits[1]][1] * (fraction);
	return (true);
}

/*
**	Clips a triangle that is intersecting a plane. The result is either one or
**	two triangles on the positive side of the plane.
*/

static t_bool		create_two_clipped_triangles(t_triangle *triangle,
											t_plane *plane, int *indices,
											t_triangle *result_tris)
{
	t_vec3 hits[2];
	t_vec2 uvs[2];
	t_ray ray1;
	t_ray ray2;
	t_vec3 dir1;
	t_vec3 dir2;

	ml_vector3_sub(triangle->vtc[indices[0]]->pos,
				   triangle->vtc[(indices[0] + 1) % 3]->pos, dir1);
	ml_vector3_sub(triangle->vtc[indices[0]]->pos,
				   triangle->vtc[(indices[0] + 2) % 3]->pos, dir2);
	l3d_ray_set(dir1, triangle->vtc[indices[0]]->pos, &ray1);
	l3d_ray_set(dir2, triangle->vtc[indices[0]]->pos, &ray2);
	l3d_plane_ray_hit(plane, &ray1, hits[0]);
	l3d_plane_ray_hit(plane, &ray2, hits[1]);
	ml_vector3_copy(triangle->vtc[(indices[0] + 1) % 3]->pos, result_tris[0].vtc[0]->pos);
	ml_vector3_copy(hits[0], result_tris[0].vtc[1]->pos);
	ml_vector3_copy(triangle->vtc[(indices[0] + 2) % 3]->pos, result_tris[0].vtc[2]->pos);
	ml_vector3_copy(triangle->vtc[(indices[0] + 2) % 3]->pos, result_tris[1].vtc[0]->pos);
	ml_vector3_copy(hits[0], result_tris[1].vtc[1]->pos);
	ml_vector3_copy(hits[1], result_tris[1].vtc[2]->pos);
	if (!(l3d_interpolate_clipped_uv(triangle, (int[2]){indices[0], (indices[0] + 1) % 3}, hits[0], uvs[0])))
		return (false);
	if (!(l3d_interpolate_clipped_uv(triangle, (int[2]){indices[0], (indices[0] + 2) % 3}, hits[1], uvs[1])))
		return (false);
	ml_vector2_copy(triangle->uvs[(indices[0] + 1) % 3], result_tris[0].uvs[0]);
	ml_vector2_copy(uvs[0], result_tris[0].uvs[1]);
	ml_vector2_copy(triangle->uvs[(indices[0] + 2) % 3], result_tris[0].uvs[2]);
	ml_vector2_copy(triangle->uvs[(indices[0] + 2) % 3], result_tris[1].uvs[0]);
	ml_vector2_copy(uvs[0], result_tris[1].uvs[1]);
	ml_vector2_copy(uvs[1], result_tris[1].uvs[2]);
	result_tris[0].clipped = true;
	result_tris[1].clipped = true;
	return (true);
}

static t_bool		create_one_clipped_triangle(t_triangle *triangle,
												t_plane *plane, int *indices,
												t_triangle *result_tris)
{
	t_vec3 hits[2];
	t_vec2 uvs[2];
	t_ray ray1;
	t_ray ray2;
	t_vec3 dir1;
	t_vec3 dir2;
	//!reverse vertex order in both clipping functions
	ml_vector3_sub(triangle->vtc[(indices[0] + 2) % 3]->pos,
				triangle->vtc[(indices[0] + 1) % 3]->pos, dir1);
	ml_vector3_sub(triangle->vtc[(indices[0] + 2) % 3]->pos,
				triangle->vtc[(indices[0]) % 3]->pos, dir2);
	l3d_ray_set(dir1, triangle->vtc[(indices[0] + 2) % 3]->pos, &ray1);
	l3d_ray_set(dir2, triangle->vtc[(indices[0] + 2) % 3]->pos, &ray2);
	l3d_plane_ray_hit(plane, &ray1, hits[0]);
	l3d_plane_ray_hit(plane, &ray2, hits[1]);
	ml_vector3_copy(triangle->vtc[(indices[0] + 2) % 3]->pos, result_tris[0].vtc[0]->pos);
	ml_vector3_copy(hits[0], result_tris[0].vtc[1]->pos);
	ml_vector3_copy(hits[1], result_tris[0].vtc[2]->pos);
	if (!(l3d_interpolate_clipped_uv(triangle, (int[2]){(indices[0] + 1) % 3, (indices[0] + 2) % 3}, hits[0], uvs[0])))
		return (false);
	if (!(l3d_interpolate_clipped_uv(triangle, (int[2]){(indices[0]) % 3, (indices[0] + 2) % 3}, hits[1], uvs[1])))
		return (false);
	ml_vector2_copy(triangle->uvs[(indices[0] + 2) % 3], result_tris->uvs[0]);
	ml_vector2_copy(uvs[0], result_tris[0].uvs[1]);
	ml_vector2_copy(uvs[1], result_tris[0].uvs[2]);
	result_tris[0].clipped = true;
	return (true);
}

/*
**	Clips a triangle that is intersecting the plane. Forms either 1 or 2 new
**	triangles depending on intersection case. Vertex attributes are interpolated
**	1. Determine the clipping case (1 or 2 triangles needed)
**	2. Calculate clipping points on plane (using plane intersection)
**	3. Interpolate vertex attributes to new points
**	4. Form triangle(s) from new points
*/

int				l3d_clip_triangle(t_triangle *triangle, t_plane *plane,
									t_triangle *result_triangles)
{
	int clip_case;
	int indices[2];

	indices[0] = 4;
	indices[1] = 4;
	clip_case = l3d_triangle_clipping_case(triangle, plane, indices);
	if (clip_case == 2)
	{
		if (!(create_two_clipped_triangles(triangle, plane, indices,
										result_triangles)))
			return (0);
		return (2);
	}
	else if (clip_case == 1)
	{
		if (!(create_one_clipped_triangle(triangle, plane, indices,
										result_triangles)))
			return (0);
		return (1);
	}
	else
	{
		return (0);
	}
}

void			l3d_set_clipped_triangles(t_vertex *vtc, t_triangle *source,
											t_triangle *dest_tris)
{
	int i;

	i = -1;
	ft_memcpy(&dest_tris[0], source, sizeof(t_triangle));
	ft_memcpy(&dest_tris[1], source, sizeof(t_triangle));
	while (++i < 3)
	{
		vtc[i].color = 0;
		vtc[3 + i].color = 0;
		ml_vector3_set_all(vtc[i].pos, 0.0);
		ml_vector3_set_all(vtc[3 + i].pos, 0.0);
		dest_tris[0].vtc[i] = &vtc[i];
		dest_tris[1].vtc[i] = &vtc[3 + i];
	}
}

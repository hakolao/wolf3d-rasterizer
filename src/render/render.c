/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/30 18:07:34 by veilo             #+#    #+#             */
/*   Updated: 2020/08/30 18:07:37 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		calculate_2d_points(t_vec2 *points_2d, t_vec3 *hits)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		points_2d[i][0] = hits[i][0];
		points_2d[i][1] = hits[i][1];
	}
}

static void		rendered_triangle_set(t_wolf3d *app,
					t_triangle *temp, t_vertex vtc[3], t_triangle *triangle)
{
	int		k;

	k = -1;
	temp->is_single_sided = triangle->is_single_sided;
	temp->material = triangle->material;
	while (++k < 3)
	{
		vtc[k].color = triangle->vtc[k]->color;
		ml_vector4_copy(triangle->vtc[k]->pos, vtc[k].pos);
		ml_vector3_copy(triangle->normals[k], temp->normals[k]);
		ml_vector2_copy(triangle->uvs[k], temp->uvs[k]);
		temp->vtc[k] = &vtc[k];
		ml_matrix4_mul_vec3(app->player.world_translation,
			temp->vtc[k]->pos, temp->vtc[k]->pos);
		ml_matrix4_mul_vec3(app->player.world_rotation,
			temp->vtc[k]->pos, temp->vtc[k]->pos);
	}
	l3d_triangle_update(temp);
}

static t_bool	screen_intersection(t_wolf3d *app, t_triangle *triangle)
{
	t_ray		rays[3];	
	t_vec3		hits[3];
	int			k;

	k = -1;
	while (++k < 3)
	{
		ml_vector3_set(hits[k], 0.0, 0.0, 0.0);
		l3d_ray_set(triangle->vtc[k]->pos, (t_vec3){0, 0, 0}, &rays[k]);
		if (!(l3d_plane_ray_hit(&app->active_scene->main_camera->screen,
			&rays[k], hits[k])))
		{
			ft_printf("Error in screen_intersection: ray from triangle\n"
						"didn't intersect with screen plane or did in too\n"
						"small an angle. Pointo of hit set to default 0.\n");
			ml_vector3_set_all(hits[k], 0);
		}
	}
	ft_memset(triangle->points_2d, 0, sizeof(float) * 6); // !remove at some point, debug only
	calculate_2d_points(triangle->points_2d, hits);
	return (true);
}

/*
**	Check it a vertex is within the viewbox with a signed distance check
*/

t_bool			vertex_between_planes(t_plane *planes, t_vertex *vertex)
{
	int		i;

	i = -1;
	while (++i < 6)
	{
		if (ml_vector3_dot(planes[i].normal, vertex->pos) + planes[i].d <= 0)
			return (false);
	}
	return (true);
}

/*
**	Check it a triangle is within the viewbox
*/

t_bool			triangle_in_view(t_wolf3d *app, t_triangle *triangle)
{
	int		i;
	t_plane	*viewplanes;

	viewplanes = app->active_scene->main_camera->viewplanes;
	i = -1;
	while(++i < 3)
	{
		if (vertex_between_planes(viewplanes, triangle->vtc[i]))
			return (true);
	}
	return (false);
}


/*
** Checks if any of triangle vertices are behind near clip distance so the
** perspective projection does not get distorted and nothing "behind" camera
** are drawn. Edit NEAR_CLIP_DIST if needed and you notice that current value
** is not enough.
*/

t_bool			triangle_behind_camera(t_triangle *triangle, t_camera *camera)
{
	if (triangle->vtc[0]->pos[2] < camera->near_clip ||
		triangle->vtc[1]->pos[2] < camera->near_clip ||
		triangle->vtc[2]->pos[2] < camera->near_clip)
			return (false);
	return (true);
}

t_bool			is_triangle_facing(t_triangle *triangle, t_vec3 dir)
{
	return (ml_vector3_dot(triangle->normal, dir) <= 0);
}

t_bool			is_rendered(t_wolf3d *app, t_triangle *triangle)
{
	t_vec3 dir;
	
	if (triangle_behind_camera(triangle, app->active_scene->main_camera))
		return (false);
	ml_vector3_sub(triangle->center, app->active_scene->main_camera->origin, dir);
	if (!is_triangle_facing(triangle, dir))
	{
		// ft_printf("is not facing camera\n");
		return (false);
	}
	return (true);
}

t_bool			is_point_behind_near(t_plane *near, t_vec3 point)
{
	t_vec3	pldir;
	t_vec3	point_from_plane;

	ml_vector3_mul(near->normal, near->d, pldir);
	ml_vector3_sub(point, pldir, point_from_plane);
	if (ml_vector3_dot(pldir, point_from_plane) < 0)
		return (true);
	return (false);
}

/*
**	Checks if the triangle clips with a plane and gives the indices for the
**	vertices that are on the positive side of the plane if it is clipping.
**	Return value is the number of vertices on the positive side of the plane.
*/

int				is_triangle_clipping(t_triangle *triangle, t_plane *plane,
									int *point_indices)
{
	int		i;
	t_plane *near = plane;

	i = -1;
	while(++i < 3)
	{
		if (is_point_behind_near(near, triangle->vtc[i]->pos))
		{
			
			if (!(is_point_behind_near(near, triangle->vtc[(i + 1) % 3]->pos)) &&
				!(is_point_behind_near(near, triangle->vtc[(i + 2) % 3]->pos)))
			{
				//case A;
				ft_printf("one is behind two are not\n");
				point_indices[0] = i;
				return (2);
			}
		}
		else
		{
			if (is_point_behind_near(near, triangle->vtc[(i + 1) % 3]->pos) &&
				is_point_behind_near(near, triangle->vtc[(i + 2) % 3]->pos))
			{
				//case B;
				ft_printf("two are behind one is not\n");
				point_indices[0] = (i + 1) % 3;
				point_indices[1] = (i + 2) % 3;
				return (1);
			}
		}
	}
	return (0);
}

t_bool			interpolate_clipped_uv(t_triangle *triangle, int *limits,
										t_vec3 hit, t_vec2 result)
{
	int		i;
	float	fraction;

	fraction = 0.0;
	i = -1;
	while (++i < 3)
	{
		if (fabs(triangle->vtc[limits[0]]->pos[i] -
				triangle->vtc[limits[1]]->pos[i]) > L3D_EPSILON)
		{
			fraction = fabs(hit[i] - triangle->vtc[limits[0]]->pos[i]) /
						fabs(triangle->vtc[limits[0]]->pos[i] -
						triangle->vtc[limits[1]]->pos[i]);
			ft_printf("fraction: %f\n", fraction);
			result[0] = triangle->uvs[limits[0]][0] * fraction +
						triangle->uvs[limits[1]][0] * (1 - fraction);
			result[1] = triangle->uvs[limits[0]][1] * fraction +
						triangle->uvs[limits[1]][1] * (1 - fraction);
			ft_printf("interpolated uv: %f  %f\n", result[0], result[1]);
			return (true);
		}
	}
	ft_printf("!this should never happen but it will if the triangle vertices"
		"are degenerate\n");
	return (false);
}

/*
**	Clips a triangle that is intersecting a plane. The result is either one or
**	two triangles on the positive side of the plane.
*/

t_bool			create_two_clipped_triangles(t_triangle *triangle,
											t_plane *plane, int *indices,
							 				t_triangle *result_tris)
{
	t_vec3 hits[2];
	t_vec2 uvs[2];
	t_ray ray1;
	t_ray ray2;
	ft_printf("create two clipped triangles\n");
	t_vec3 dir1;
	t_vec3 dir2;

	ml_vector3_sub(triangle->vtc[indices[0]]->pos,
					triangle->vtc[(indices[0] + 1) % 3]->pos, dir1);
	ml_vector3_sub(triangle->vtc[indices[0] % 3]->pos,
					triangle->vtc[(indices[0] + 2) % 3]->pos, dir2);
	l3d_ray_set(dir1, triangle->vtc[(indices[0] + 1) % 3]->pos, &ray1);
	l3d_ray_set(dir2, triangle->vtc[(indices[0] + 2) % 3]->pos, &ray2);
	l3d_plane_ray_hit(plane, &ray1, hits[0]);
	l3d_plane_ray_hit(plane, &ray2, hits[1]);
	ft_printf("-----------------------\nhits and vertices\n");
	ml_vector3_print(hits[0]);
	ml_vector3_print(hits[1]);
	ml_vector3_print(triangle->vtc[indices[0]]->pos);
	ml_vector3_print(triangle->vtc[(indices[0] + 1) % 3]->pos);
	ml_vector3_print(triangle->vtc[(indices[0] + 2) % 3]->pos);
	ft_printf("-----------------------\n");
	//??copy first triangle using hit from ab(?)
	ml_vector3_copy(triangle->vtc[(indices[0] + 1) % 3]->pos, result_tris[0].vtc[0]->pos);
	ml_vector3_copy(hits[1], result_tris[0].vtc[2]->pos);
	ml_vector3_copy(hits[0], result_tris[0].vtc[1]->pos);

	//??copy second triangle using hit from ac(?)
	ml_vector3_copy(triangle->vtc[(indices[0] + 2) % 3]->pos, result_tris[1].vtc[0]->pos);
	ml_vector3_copy(hits[1], result_tris[1].vtc[1]->pos);
	ml_vector3_copy(triangle->vtc[(indices[0] + 1) % 3]->pos, result_tris[1].vtc[2]->pos);

	if (!(interpolate_clipped_uv(triangle, (int[2]){indices[0],
									(indices[0] + 1) % 3}, hits[0], uvs[0])))
		return (false);
	if (!(interpolate_clipped_uv(triangle, (int[2]){indices[0],
									(indices[0] + 2) % 3}, hits[1], uvs[1])))
		return (false);
	ml_vector2_copy(triangle->uvs[(indices[0] + 1) % 3], result_tris[0].uvs[0]);
	ml_vector2_copy(uvs[0], result_tris[0].uvs[1]);
	ml_vector2_copy(uvs[1], result_tris[0].uvs[2]);
	ml_vector2_copy(triangle->uvs[(indices[0] + 2) % 3], result_tris[1].uvs[0]);
	ml_vector2_copy(uvs[1], result_tris[1].uvs[1]);
	ml_vector2_copy(uvs[0], result_tris[1].uvs[2]);
	
	return true;
}

t_bool			create_one_clipped_triangle(t_triangle *triangle,
											t_plane *plane, int *indices,
							 				t_triangle *result_tris)
{
	t_vec3	hits[2];
	t_vec2	uvs[2];
	t_ray	ray1;
	t_ray	ray2;
	t_vec3	dir1;
	t_vec3	dir2;

	ft_printf("create one clipped triangle\n");
	ml_vector3_sub(triangle->vtc[(indices[0] + 1) % 3]->pos,
					triangle->vtc[indices[0]]->pos, dir1);
	ml_vector3_sub(triangle->vtc[(indices[0] + 2) % 3]->pos,
					triangle->vtc[indices[0]]->pos, dir2);
	l3d_ray_set(dir1, triangle->vtc[indices[0]]->pos, &ray1);
	l3d_ray_set(dir2, triangle->vtc[indices[0]]->pos, &ray2);
	l3d_plane_ray_hit(plane, &ray1, hits[0]);
	l3d_plane_ray_hit(plane, &ray2, hits[1]);
	ml_vector3_copy(triangle->vtc[indices[0]]->pos, result_tris[0].vtc[0]->pos);
	ml_vector3_copy(hits[0], result_tris[0].vtc[1]->pos);
	ml_vector3_copy(hits[1], result_tris[0].vtc[2]->pos);
	if (!(interpolate_clipped_uv(triangle, (int[2]){indices[0],
									(indices[0] + 1) % 3}, hits[0], uvs[0])))
		return (false);
	if (!(interpolate_clipped_uv(triangle, (int[2]){indices[0],
									(indices[0] + 2) % 3}, hits[1], uvs[1])))
		return (false);
	ml_vector2_copy(triangle->uvs[indices[0]], result_tris->uvs[0]);
	ml_vector2_copy(uvs[0], result_tris->uvs[1]);
	ml_vector2_copy(uvs[1], result_tris->uvs[2]);
	// int i = -1;
	// while(++i < 3)
	// {
	// 	ml_vector3_print(result_tris->vtc[i]->pos);
	// }
	return (true);
}

t_bool			clip_triangle(t_triangle *triangle, t_plane *plane,
								t_triangle *result_triangles)
{
	// t_triangle	*clip1;
	// t_triangle	*clip2;//will be part of result as input
	int			clip_case;
	int			indices[2];

	indices[0] = 4;
	indices[1] = 4;
	//?? 1. Determine the clipping case
	//?? 2. Calculate clipping points on plane
	//?? 3. Interpolate vertex attributes to new points
	//?? 4. Form triangle(s) from new points
	//?? 5. Call render for new triangle(s)
	clip_case = is_triangle_clipping(triangle, plane, indices);
	if (clip_case == 2)
	{
		if (!(create_two_clipped_triangles(triangle, plane, indices,
											result_triangles)))
			return (false);
		return (true);
	}
	else if (clip_case == 1)
	{
		if (!(create_one_clipped_triangle(triangle, plane, indices,
											result_triangles)))
			return (false);
		return (true);
	}
	else
		return (false);
}

void			set_clipped_triangles(t_vertex *vtc, t_triangle *source,
										t_triangle *dest_tris)
{
	int		i;

	i = -1;
	dest_tris[0].material = source->material;
	dest_tris[1].material = source->material;
	dest_tris[0].is_single_sided = source->is_single_sided;
	dest_tris[1].is_single_sided = source->is_single_sided;
	while (++i < 3)
	{
		vtc[3 + i].color = 0;
		vtc[6 + i].color = 0;
		ml_vector3_copy(source->normals[i], dest_tris[0].normals[i]);
		ml_vector3_copy(source->normals[i], dest_tris[1].normals[i]);
		ml_vector2_copy(source->uvs[i], dest_tris[0].uvs[i]);
		ml_vector2_copy(source->uvs[i], dest_tris[1].uvs[i]);
		ml_vector3_set_all(vtc[3 + i].pos, 0.0);
		ml_vector3_set_all(vtc[6 + i].pos, 0.0);
		dest_tris[0].vtc[i] = &vtc[3 + i];
		dest_tris[1].vtc[i] = &vtc[6 + i];
	}
}

/*
**	Renders a triangle using a temporary triangle that is transformed to the
**	actual location in the world. Also creates temporary clipping triangles
**	if the triangle intersects with the camera near plane.
**	The vtc[9] array is shared between the temporary triangles
*/

void			print_clipped_triangles(t_triangle *triangles)
{
	int i = -1;
	ft_printf("----------------\nclipped triangles---------------\n");
	while (++i < 3)
	{
		ft_printf("pos: ");
		ml_vector3_print(triangles[0].vtc[i]->pos);
		ft_printf("uv: ");
		ml_vector2_print(triangles[0].uvs[i]);
	}
	ft_printf("------------------------------\n");
	i = -1;
	while (++i < 3)
	{
		ft_printf("pos: ");
		ml_vector3_print(triangles[1].vtc[i]->pos);
		ft_printf("uv: ");
		ml_vector2_print(triangles[1].uvs[i]);
	}
}

t_bool			render_triangle(t_wolf3d *app, t_triangle *triangle_in)
{
	uint32_t	*buffer;
	uint32_t	dimensions[2];
	t_triangle	render_triangle;
	t_triangle	clipped_triangles[2];
	t_vertex	vtc[9];

	buffer = app->window->framebuffer;
	dimensions[0] = app->window->width;
	dimensions[1] = app->window->height;
	rendered_triangle_set(app, &render_triangle, vtc, triangle_in);
	// if (!(triangle_in_view(app, &render_triangle)))
	// 	return (false);
	// if (!(is_rendered(app, &render_triangle)))
	// 	return (false);
	set_clipped_triangles(vtc, &render_triangle, clipped_triangles);
	if (clip_triangle(&render_triangle,
					&app->active_scene->main_camera->viewplanes[0],
					clipped_triangles))
	{
		ft_printf("clip triangle\n");
		screen_intersection(app, &clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[1]);
		l3d_triangle_raster(buffer, dimensions, &clipped_triangles[0]);
		l3d_triangle_raster(buffer, dimensions, &clipped_triangles[1]);
	}
	else
	{
		print_clipped_triangles(clipped_triangles);
		screen_intersection(app, &render_triangle);
		l3d_triangle_raster(buffer, dimensions, &render_triangle);
	}
	return (true);
}
// t_vertex *vtc[3];
// t_vec2 uvs[3];
// t_vec3 normals[3];
// t_vec3 center;
// t_vec3 normal;
// t_bool is_single_sided;
// t_vec3 ab;
// t_vec3 ac;
// t_vertex *ordered_vtc[3];
// t_material *material;
// t_vec2 points_2d[3];
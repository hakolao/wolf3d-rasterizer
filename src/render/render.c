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

t_bool			screen_intersection(t_wolf3d *app, t_triangle *triangle)
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
			// ft_printf("Error in screen_intersection: ray from triangle\n"
			// 			"didn't intersect with screen plane or did in too\n"
			// 			"small an angle. Pointo of hit set to default 0.\n"
			// 			"Might happen due to degenerate or badly defined\n"
			// 			"triangles.\n");
			ml_vector3_set_all(hits[k], 0);
		}
	}
	calculate_2d_points(triangle->points_2d, hits);
	return (true);
}

/*
** Checks if any of triangle vertices are behind near clip distance so the
** perspective projection does not get distorted and nothing "behind" camera
** are drawn. Edit NEAR_CLIP_DIST if needed and you notice that current value
** is not enough.
*/

t_bool			triangle_behind_camera(t_triangle *triangle, t_camera *camera)
{
	if (triangle->vtc[0]->pos[2] > camera->near_clip &&
		triangle->vtc[1]->pos[2] > camera->near_clip &&
		triangle->vtc[2]->pos[2] > camera->near_clip)
			return (true);
	return (false);
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
	// if (!is_triangle_facing(triangle, dir))
	// {
	// 	return (false);
	// }
	return (true);
}

/*
**	Renders a triangle using a temporary triangle that is transformed to the
**	actual location in the world. Also creates temporary clipping triangles
**	if the triangle intersects with the camera near plane.
**	The vtc[9] array is shared between the temporary triangles
*/

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
	if (!(is_rendered(app, &render_triangle)))
		return (false);
	l3d_set_clipped_triangles(vtc, &render_triangle, clipped_triangles);
	//!^make to set only once per frame	//!if possible
	if (l3d_clip_triangle(&render_triangle,
					&app->active_scene->main_camera->viewplanes[0],
					clipped_triangles))
	{
		screen_intersection(app, &clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[1]);
		l3d_triangle_raster(buffer, dimensions, &clipped_triangles[0]);
		l3d_triangle_raster(buffer, dimensions, &clipped_triangles[1]);
		return (true);
	}
	else
	{
		screen_intersection(app, &render_triangle);
		l3d_triangle_raster(buffer, dimensions, &render_triangle);
	}

	return (true);
}

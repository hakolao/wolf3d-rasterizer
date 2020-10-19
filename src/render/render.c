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
		// if (points_2d[i][0] > WIDTH / 2 || points_2d[i][1] > HEIGHT / 2 ||
		// 	points_2d[i][0] < -WIDTH / 2 || points_2d[i][1] < -HEIGHT / 2)
		// 		ft_printf("points2d out of bounds: x: %f | y: %f\n", points_2d[i][0], points_2d[i][1]);
				//!remove print after debug
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

static t_bool 	triangle_behind_camera(t_triangle *triangle, t_camera *camera)
{
	if (triangle->vtc[0]->pos[2] < camera->near_clip &&
		triangle->vtc[1]->pos[2] < camera->near_clip &&
		triangle->vtc[2]->pos[2] < camera->near_clip)
		return (false);
	return (true);
}

static t_bool	is_triangle_facing(t_triangle *triangle, t_vec3 dir)
{
	return (ml_vector3_dot(triangle->normal, dir) <= 0);
}

static t_bool	is_rendered(t_wolf3d *app, t_triangle *triangle)
{
	t_vec3 dir;
	
	if (triangle_behind_camera(triangle, app->active_scene->main_camera))
		return (false);
	ml_vector3_sub(triangle->center, app->active_scene->main_camera->origin, dir);
	if (!is_triangle_facing(triangle, dir))
		return (false);
	return (true);
}

t_bool			render_triangle(t_wolf3d *app, t_triangle *triangle_in)
{
	uint32_t	*buffer;
	uint32_t	dimensions[2];
	t_triangle	render_triangle;
	t_vertex	vtc[3];
	
	rendered_triangle_set(app, &render_triangle, vtc, triangle_in);
	if (!(triangle_in_view(app, &render_triangle)))
		return (false);
	if (!(is_rendered(app, &render_triangle)))
		return (false);
	buffer = app->window->framebuffer; 
	dimensions[0] = app->window->width;
	dimensions[1] = app->window->height;
	screen_intersection(app, &render_triangle);
	l3d_triangle_raster(buffer, dimensions, &render_triangle);
	return (true);
}

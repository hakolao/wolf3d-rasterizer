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
	ft_memcpy(temp, triangle, sizeof(t_triangle));
	while (++k < 3)
	{
		ft_memcpy(&vtc[k], triangle->vtc[k], sizeof(t_vertex));
		temp->vtc[k] = &vtc[k];
		ml_matrix4_mul_vec3(app->player.translation,
			temp->vtc[k]->pos, temp->vtc[k]->pos);
		ml_matrix4_mul_vec3(app->player.inv_rotation,
			temp->vtc[k]->pos, temp->vtc[k]->pos);
	}
	l3d_triangle_update(temp);
}

static t_bool	screen_intersection(t_wolf3d *app, t_triangle *triangle,
								t_vec2 *points_2d)
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
	
	calculate_2d_points(points_2d, hits);
	return (true);
}

void			draw_debug_crosshair_on_corners(t_wolf3d *app, t_vec2 *ordered_corners)
{
	uint32_t width = app->main_window->width;
	uint32_t height = app->main_window->height;

	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[0][0] + width / 2, ordered_corners[0][1] + height / 2},
							  {ordered_corners[0][0] + width / 2, ordered_corners[0][1] + 20 + height / 2}},
				  0x00ff00ff);
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[0][0] + width / 2, ordered_corners[0][1] + height / 2},
							  {ordered_corners[0][0] + 20 + width / 2, ordered_corners[0][1] + height / 2}},
				  0x00ff00ff);

	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[1][0] + width / 2, ordered_corners[1][1] + height / 2},
							  {ordered_corners[1][0] + width / 2, ordered_corners[1][1] + 20 + height / 2}},
				  0xff0000ff);
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[1][0] + width / 2, ordered_corners[1][1] + height / 2},
							  {ordered_corners[1][0] + 20 + width / 2, ordered_corners[1][1] + height / 2}},
				  0xff0000ff);

	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[2][0] + width / 2, ordered_corners[2][1] + height / 2},
							  {ordered_corners[2][0] + width / 2, ordered_corners[2][1] + 20 + height / 2}},
				  0xfff0f0ff);
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[2][0] + width / 2, ordered_corners[2][1] + height / 2},
							  {ordered_corners[2][0] + 20 + width / 2, ordered_corners[2][1] + height / 2}},
				  0xfff0f0ff);
}

/*
**	Check it a vertex is within the viewbox with a signed distance check
*/

t_bool			vertex_in_box(t_wolf3d *app, t_plane *planes, t_vertex *vertex)
{
	int		i;
	t_vec3	offset_plane_normal;

	i = -1;
	ml_vector3_add(planes[i].normal, planes->origin, offset_plane_normal);
	while (++i < 4)
	{
		ml_vector3_add(planes[i].normal, planes->origin, offset_plane_normal);
		if (ml_vector3_dot(offset_plane_normal, vertex->pos) < 0)
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
		if (vertex_in_box(app, viewplanes, triangle->vtc[i]))
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
	t_vec2		points_2d[3];
	uint32_t	*buffer;
	uint32_t	dimensions[2];
	t_triangle	render_triangle;
	t_vertex	vtc[3];

	rendered_triangle_set(app, &render_triangle, vtc, triangle_in);
	if (!(is_rendered(app, &render_triangle)))
		return (false);
	buffer = app->main_window->framebuffer; 
	dimensions[0] = app->main_window->width;
	dimensions[1] = app->main_window->height;
	ft_memset(points_2d, 0, sizeof(points_2d));
	screen_intersection(app, &render_triangle, points_2d);
	l3d_triangle_raster(buffer, dimensions, &render_triangle, points_2d);
	return (true);
}

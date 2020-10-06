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
#define BREAKLIMIT 1000//!debug only

void		calculate_2d_points(t_vec2 *points_2d, t_vec3 *hits)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		points_2d[i][0] = hits[i][0];
		points_2d[i][1] = hits[i][1];
	}
}

t_bool		screen_intersection(t_wolf3d *app, t_triangle *triangle,
								t_vec2 *points_2d)
{
	t_ray	rays[3];	
	t_vec3	hits[3];
	int k;
	t_plane screen;

	k = -1;
	ml_vector3_set(screen.origin, 0.0, 0.0,
					-app->active_scene->main_window->width);
	ml_vector3_set(screen.normal, 0.0, 0.0, 1.0);
	while (++k < 3)
	{
		ml_vector3_set(hits[k], 0.0, 0.0, 0.0);
		l3d_ray_set(triangle->vtc[k]->pos, (t_vec3){0.0, 0.0, 0.0}, &rays[k]);
		if (!(plane_ray_intersect(&screen, &rays[k], hits[k])))
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

void		draw_debug_crosshair_on_corners(t_wolf3d *app, t_vec2 *ordered_corners)
{
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[0][0], ordered_corners[0][1]},
							  {ordered_corners[0][0], ordered_corners[0][1] + 20}},
				  0x00ff00ff);
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[0][0], ordered_corners[0][1]},
							  {ordered_corners[0][0] + 20, ordered_corners[0][1]}},
				  0x00ff00ff);

	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[1][0], ordered_corners[1][1]},
							  {ordered_corners[1][0], ordered_corners[1][1] + 20}},
				  0xff0000ff);
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[1][0], ordered_corners[1][1]},
							  {ordered_corners[1][0] + 20, ordered_corners[1][1]}},
				  0xff0000ff);

	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[2][0], ordered_corners[2][1]},
							  {ordered_corners[2][0], ordered_corners[2][1] + 20}},
				  0xfff0f0ff);
	l3d_line_draw(app->main_window->rbuffer, (uint32_t[2]){WIDTH, HEIGHT},
				  (int[2][2]){{ordered_corners[2][0], ordered_corners[2][1]},
							  {ordered_corners[2][0] + 20, ordered_corners[2][1]}},
				  0xfff0f0ff);
}

/*
** Checks if any of triangle vertices are behind near clip distance so the
** perspective projection does not get distorted and nothing "behind" camera
** are drawn. Edit NEAR_CLIP_DIST if needed and you notice that current value
** is not enough.
*/

static		t_bool 	triangle_behind_camera(t_triangle *triangle, t_camera *camera)
{
	if (triangle->vtc[0]->pos[2] < camera->near_clip &&
		triangle->vtc[1]->pos[2] < camera->near_clip &&
		triangle->vtc[2]->pos[2] < camera->near_clip)
		return (false);
	return (true);
}

static		t_bool	is_triangle_facing(t_triangle *triangle, t_vec3 dir)
{
	return (ml_vector3_dot(triangle->normal, dir) <= 0);
}

t_bool		is_rendered(t_wolf3d *app, t_triangle *triangle)
{
	t_vec3 dir;
	
	if (triangle_behind_camera(triangle, app->active_scene->main_camera))
		return (false);
	ml_vector3_sub(triangle->center, app->active_scene->main_camera->origin, dir);
	if (!is_triangle_facing(triangle, dir))
		return (false);
	return (true);
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle)//?App specific implementation
// t_bool			render_triangle(dimensionswh, triangle, buffer, 2d_points)
{
	t_vec2		points_2d[3];
	int			i;
	uint32_t	*buffer;
	uint32_t	dimensions[2];
	
	if (!(is_rendered(app, triangle)))
		return (false);
	buffer = app->main_window->rbuffer;
	dimensions[0] = app->main_window->width;
	dimensions[1] = app->main_window->height;
	screen_intersection(app, triangle, points_2d);
	l3d_triangle_raster(buffer, dimensions, triangle, points_2d); //?expose this
	// draw_debug_crosshair_on_corners(app, ordered_corners);
	i = -1;
	while (++i < app->main_window->width * app->main_window->height)
		app->main_window->framebuffer[i] = app->main_window->rbuffer[i];
	return (true);
}

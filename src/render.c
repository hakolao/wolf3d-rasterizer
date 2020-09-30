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

//SERIOUSLY consider if vertices should be in contiguous array
//and triangles just indexing them
//that would make looping vertices much easier

//add baryocentric coordinatesystem to triangle for easier
//fragment shader use

#include "wolf3d.h"

void		screen_intersection(t_camera *camera, t_triangle *triangle,
				t_vec2 *corners_on_screen)
{
	t_ray	rays[3];
	int		i;
	float	scaler;

	i = -1;
	while (++i < 3)
	{
		l3d_ray_set(triangle->vtc[i]->pos, (t_vec3){0.0, 0.0, 0.0}, &rays[i]);
		scaler = (camera->screen_dist / rays[i].dir[2]);
		rays[i].dir[0] *= scaler;
		rays[i].dir[1] *= scaler;
		ml_vector2_copy((t_vec2){rays[i].dir[0], rays[i].dir[1]},
			corners_on_screen[i]);
	}
}

/*
** Checks if any of triangle vertices are behind near clip distance so the
** perspective projection does not get distorted and nothing "behind" camera
** are drawn. Edit NEAR_CLIP_DIST if needed and you notice that current value
** is not enough.
*/

t_bool		triangle_behind_camera(t_triangle *triangle)
{
	if (triangle->vtc[0]->pos[2] < NEAR_CLIP_DIST &&
		triangle->vtc[1]->pos[2] < NEAR_CLIP_DIST &&
		triangle->vtc[2]->pos[2] < NEAR_CLIP_DIST)
		return (false);
	return (true);
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle)
{
	t_vec2				corners_on_screen[3];
	t_vec2				corners[3];
	int					i;

	if (triangle_behind_camera(triangle))
		return (false);
	screen_intersection(app->active_scene->main_camera, triangle,
		corners_on_screen);
	i = -1;
	while (++i < 3)
	{
		corners[i][0] = corners_on_screen[i][0] + app->main_window->width / 2;
		corners[i][1] = corners_on_screen[i][1] + app->main_window->height / 2;
	}
	l3d_triangle_edges_draw(app->main_window->rbuffer,
		(uint32_t[2]){app->main_window->width,
		app->main_window->height},
		corners, app->main_window->rbuf_render_color);
	i = -1;
	while (++i < app->main_window->width * app->main_window->height)
		app->main_window->framebuffer[i] = app->main_window->rbuffer[i];
	return (true);
}

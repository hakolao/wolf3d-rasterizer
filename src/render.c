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

/*
**	Converts pixel position from screen coordinates to frame buffer index
**	Left top corner is considered (0,0) and bottom right (width, height)
*/

int			screen_to_frame_coords(uint32_t width, uint32_t height, int x, int y)
{
	x *= -1;
	y *= -1;
	x += width;
	y += height;
	return (x + y * width);
}

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

void		draw_triangle_edges(t_wolf3d *app, int *ints_on_screen, uint32_t color)
{
	uint32_t	dimensions[2];

	dimensions[0] = app->main_window->width;
	dimensions[1] = app->main_window->height;
	l3d_line_draw(app->main_window->rbuffer, dimensions,
		(int32_t[2][2]){{ints_on_screen[0], ints_on_screen[1]},
			{ints_on_screen[2], ints_on_screen[3]}}, color);
	l3d_line_draw(app->main_window->rbuffer, dimensions,
		(int32_t[2][2]){{ints_on_screen[0], ints_on_screen[1]},
			{ints_on_screen[4], ints_on_screen[5]}}, color);
	l3d_line_draw(app->main_window->rbuffer, dimensions,
		(int32_t[2][2]){{ints_on_screen[2], ints_on_screen[3]},
			{ints_on_screen[4], ints_on_screen[5]}}, color);
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
							t_camera *camera)
{
	t_vec2				corners_on_screen[3];
	uint32_t			*rbuffer;
	int					ints_on_screen[6];

	rbuffer = app->main_window->rbuffer;
	screen_intersection(app->active_scene->main_camera, triangle, corners_on_screen);
	int j = -1;
	while (++j < 3)
	{
		ints_on_screen[j * 2] =
			(int)corners_on_screen[j][0] + app->main_window->width / 2;
		ints_on_screen[j * 2 + 1] =
			(int)corners_on_screen[j][1] + app->main_window->height / 2;
	}
	//!DRAW ORDER: AB, BC, CA
	draw_triangle_edges(app, ints_on_screen, app->main_window->rbuf_render_color);
	int k = 0;
	while (k < app->main_window->width *  app->main_window->height)
	{
		app->main_window->framebuffer[k] = rbuffer[k];
		k++;
	}
	(void)camera;
	return (true);
}

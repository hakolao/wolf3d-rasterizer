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

#define EDGE_DELAY 2 //TODO REMOVE
#define FILL_SPEED 1
#define ANIMATION 0
#define FILL_ANIMATION 0
#define RANDOM_COLOR 0

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

void		screen_intersection(t_wolf3d *app, t_triangle *triangle,
								t_vec2 *corners_on_screen)
{
	t_ray	rays[3];
	int		i;
	float	scaler;

	i = -1;
	while (++i < 3)
	{
		rays[i] = new_ray((t_vec3){0.0, 0.0, 0.0}, triangle->vtc[i]->position);
		scaler = (app->active_scene->main_camera->screen_dist / rays[i].dir[0]);
		rays[i].dir[1] *= scaler;
		rays[i].dir[2] *= scaler;
		ml_vector2_copy((t_vec2){rays[i].dir[1], rays[i].dir[2]},
						corners_on_screen[i]);
	}
}

void		calculate_triangle_center(t_triangle *triangle,
										int *triangle_center)
{
	triangle_center[0] = (int)((triangle->vtc[0]->position[1] +
						triangle->vtc[1]->position[1] +
						triangle->vtc[2]->position[1]) / 3);
	triangle_center[1] = (int)((triangle->vtc[0]->position[2] +
								triangle->vtc[1]->position[2] +
								triangle->vtc[2]->position[2]) /
							   3);
}


void		draw_triangle_edges(t_wolf3d *app, int *ints_on_screen, uint32_t color)
{
		draw_line(	(int[2]){ints_on_screen[0] + WIDTH / 2,
							ints_on_screen[1] + HEIGHT / 2},
					(int[2]){ints_on_screen[2] + WIDTH / 2,
							ints_on_screen[3] + HEIGHT / 2}, color, app);
		draw_line(	(int[2]){ints_on_screen[0] + WIDTH / 2,
							ints_on_screen[1] + HEIGHT / 2},
					(int[2]){ints_on_screen[4] + WIDTH / 2,
							ints_on_screen[5] + HEIGHT / 2}, color, app);
		draw_line(	(int[2]){ints_on_screen[2] + WIDTH / 2,
							ints_on_screen[3] + HEIGHT / 2},
					(int[2]){ints_on_screen[4] + WIDTH / 2,
							ints_on_screen[5] + HEIGHT / 2}, color, app);
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
					   t_mesh *mesh, t_camera *camera)
{
	t_vec2				corners_on_screen[3];
	uint32_t			*rbuffer;
	int					width = app->main_window->width;
	int					height = app->main_window->height;
	int					ints_on_screen[6];

	rbuffer = app->main_window->rbuffer;
	(void)mesh;
	screen_intersection(app, triangle, corners_on_screen);
	int j = -1;
	while (++j < 3)
	{
		ints_on_screen[j * 2] = (int)corners_on_screen[j][0];
		ints_on_screen[j * 2 + 1] = (int)corners_on_screen[j][1];
	}
	//!DRAW ORDER: AB, BC, CA
	draw_triangle_edges(app, ints_on_screen, app->main_window->rbuf_render_color);
	int k = 0;
	while (k < width * height)
	{
		app->main_window->framebuffer[k] = rbuffer[k];
		k++;
	}
	(void)camera;
	return (true);
}

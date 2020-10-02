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
#define BREAKLIMIT 1500
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
	draw_line(	(int[2]){ints_on_screen[0],
						ints_on_screen[1]},
				(int[2]){ints_on_screen[2],
						ints_on_screen[3]}, color, app);
	draw_line(	(int[2]){ints_on_screen[0],
						ints_on_screen[1]},
				(int[2]){ints_on_screen[4],
						ints_on_screen[5]}, color, app);
	draw_line(	(int[2]){ints_on_screen[2],
						ints_on_screen[3]},
				(int[2]){ints_on_screen[4],
						ints_on_screen[5]}, color, app);
}

void	order_vertices_y(t_triangle *triangle, t_vertex **vtc, t_vec2 *corners_on_screen) //?static??
{
	size_t indices[3];
	ft_max_double_idx((double[3]){triangle->vtc[1]->pos[1],
								  triangle->vtc[2]->pos[1],
								  triangle->vtc[0]->pos[1]},
					  3, &indices[0]);
	vtc[2] = triangle->vtc[indices[0]];
	ft_min_double_idx((double[3]){triangle->vtc[1]->pos[1],
								  triangle->vtc[2]->pos[1],
								  triangle->vtc[0]->pos[1]},
					  3, &indices[2]);
	vtc[0] = triangle->vtc[indices[2]];
	vtc[1] = triangle->vtc[3 - (indices[0] + indices[2])];
	// ml_vector3_print(vtc[0]->pos);
	// ml_vector3_print(vtc[1]->pos);
	// ml_vector3_print(vtc[2]->pos);
	(void)corners_on_screen;
}

void	raster_upper(t_wolf3d *app, t_vertex **vtc, t_vec2 *corners_on_screen, t_camera *camera)
{
	float x;
	float y;
	float x1 = vtc[0]->pos[0];
	float x2 = vtc[1]->pos[0];
	float x3 = vtc[2]->pos[0];
	float y1 = vtc[0]->pos[1];
	float y2 = vtc[1]->pos[1];
	float y3 = vtc[2]->pos[1];
	int width = app->main_window->width;
	int height = app->main_window->height;

	y = y1;
	x = y2;
	int i = 0;
	while ((int)y != (int)y2) // from {0;1} to {0;2}
	{
		y += (y2 - y) / (fabs((float)((y2) - y)));
		x = x1 + (x2 - x1) * ((y - y1) / (y2 - y1));
		float end_x = x1 + (x3 - x1) * ((y - y1) / (y3 - y1));
		while ((int)x != (int)end_x)
		{
			app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width, app->main_window->height,
			x + width / 2, y + height / 2)] = app->main_window->rbuf_render_color;
			// x, y
			// rbuffer[x,y] = color;

			x += (x1 - x2) / (fabs(x1 - x2));
			if (i++ > BREAKLIMIT) //?prevents inf loops in testing mode
			{
				// printf("break1\n");
				break;
			}
		}
	}
	(void)camera;
	(void)corners_on_screen;
}

void	raster_lower(t_wolf3d *app, t_vertex **vtc, t_vec2 *corners_on_screen, t_camera *camera)
{
	float	x;
	float	y;
	float x1 = vtc[0]->pos[0];
	float x2 = vtc[1]->pos[0];
	float x3 = vtc[2]->pos[0];
	float y1 = vtc[0]->pos[1];
	float y2 = vtc[1]->pos[1];
	float y3 = vtc[2]->pos[1];
	int width = app->main_window->width;
	int height = app->main_window->height;
	y = y2;
	x = x2;
	int i = 0;
	while ((int)y != (int)y3) // from {1;2} to {0;2}
	{
		y += (y3 - y2) / (fabs((float)((y3)-y2)));
		x = x2 + (x3 - x2) * ((y - y2) / (y3 - y2));
		float end_x = x1 + (x3 - x1) * ((y - y1) / (y3 - y1));
		while ((int)x != (int)end_x)
		{
			app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width, app->main_window->height,
			x + width / 2, y + height / 2)] = app->main_window->rbuf_render_color;

			x += (x3 - x2) / (fabs(x3 - x2));
			if (i++ > BREAKLIMIT) //?prevents inf loops in testing mode
			{
				// printf("break2\n");
				break;
			}
		}
	}
	(void)camera;
	(void)corners_on_screen;
}

void	rasterize_triangle(t_wolf3d *app, t_triangle *triangle, t_vec2 *corners_on_screen, t_camera *camera)
{
	t_vertex *vtc[3];
	order_vertices_y(triangle, vtc, corners_on_screen); //?static??
	raster_upper(app, vtc, corners_on_screen, camera);
	raster_lower(app, vtc, corners_on_screen, camera);

	/*
	** sort vertices in height order in an array
	** find y values for every x value for lines {0;1}, {1;2}, {0;2}
	** scan horrizontal lines from point x,y on lines {0;1} and {1;2}
	** to x,y point on line {0;2}
	** using formula a to b
	** a += (b-a)/(abs()b-a) until a == b
	*/
	(void)camera;
	(void)app;
	// (void)x;
	(void)triangle;
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
							t_camera *camera)
{
	
	t_vec2				corners_on_screen[3];
	uint32_t			*rbuffer;
	int					ints_on_screen[6];
	rbuffer = app->main_window->rbuffer;
	screen_intersection(app->active_scene->main_camera, triangle, corners_on_screen);
	rasterize_triangle(app, triangle, corners_on_screen, camera);
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

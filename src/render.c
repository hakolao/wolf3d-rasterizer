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
#define BREAKLIMIT 100000
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



void	order_corners_y(t_triangle *triangle, t_vertex **vtc, t_vec2 *ordered_corners, t_vec2 *corners_on_screen) //?static??
{
	size_t indices[3];
	ft_min_double_idx((double[3]){corners_on_screen[0][1],
								  corners_on_screen[1][1],
								  corners_on_screen[2][1]},
					  3, &(indices[0]));
	vtc[0] = triangle->vtc[indices[0]];
	ml_vector2_copy(corners_on_screen[indices[0]], ordered_corners[0]);//!
	ft_max_double_idx((double[3]){corners_on_screen[0][1],
								  corners_on_screen[1][1],
								  corners_on_screen[2][1]},
					  3, &(indices[2]));
	vtc[2] = triangle->vtc[indices[2]];
	ml_vector2_copy(corners_on_screen[indices[2]], ordered_corners[2]); //!

	indices[1] = 3 - (indices[0] + indices[2]);
	vtc[1] = triangle->vtc[indices[1]];
	ml_vector2_copy(corners_on_screen[indices[1]], ordered_corners[1]);//!
	(void)corners_on_screen;
}

uint32_t	get_color_normal(t_triangle *triangle)
{
	t_vec3		normal;
	uint32_t	color;

	ml_vector3_normalize(triangle->normal, normal);
	color = 0x0;
	color += (uint32_t)(255 * fabs(normal[0])) << 8;
	color += (uint32_t)(255 * fabs(normal[1])) << 16;
	color += (uint32_t)(255 * fabs(normal[2])) << 24;
	return (color);
}

void scan_line(t_wolf3d *app, float *limits, t_vertex **vtc, t_triangle *triangle)
{
	int i = 0;
	int width = app->main_window->width;
	int height = app->main_window->height;
	int x;
	int y;
	int end_x;
	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		l3d_pixel_plot(app->main_window->rbuffer,
					   (uint32_t[2]){width, height},
					   (int[2]){x + width / 2, y + height / 2},
					   get_color_normal(triangle));
		x++;
		if (i++ > BREAKLIMIT) //?prevents inf loops in testing mode
		{
			// printf("break2\n");
			break;
		}
	}
	(void)vtc;
}

typedef struct	s_raster_data
{
	float	x1;
	float	x2;
	float	x3;
	float	y1;
	float	y2;
	float	y3;
	float	slope_ab;
	float	slope_bc;
	float	slope_ac;
}				t_raster_data;

void	raster_upper(t_wolf3d *app, t_vertex **vtc, t_triangle *triangle, t_raster_data *data)
{
	float x;
	float y;
	float end_x;

	y = data->y1;
	while (y < data->y2)
	{
		x = data->x2 + data->slope_ab * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (x < end_x)
			scan_line(app, (float[3]){x, end_x + 1, y}, vtc, triangle);
		else
			scan_line(app, (float[3]){end_x, x + 1, y}, vtc, triangle);
		y++;
	}
}

void	raster_lower(t_wolf3d *app, t_vertex **vtc, t_triangle *triangle, t_raster_data *data)
{
	float x;
	float y;
	float end_x;

	y = data->y2;
	while (y < data->y3)
	{
		x = data->x2 + data->slope_bc * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (x < end_x)
			scan_line(app, (float[3]){x, end_x + 1, y}, vtc, triangle);
		else
			scan_line(app, (float[3]){end_x, x + 1, y}, vtc, triangle);
		y++;
	}
}

void	rasterize_triangle(t_wolf3d *app, t_triangle *triangle, t_vec2 *ordered_corners,
							t_vec2 *corners_on_screen, t_camera *camera)
{
	t_vertex *vtc[3];
	t_raster_data data;

	order_corners_y(triangle, vtc, ordered_corners, corners_on_screen);
	data.x1 = floor(ordered_corners[0][0]);
	data.x2 = floor(ordered_corners[1][0]);
	data.x3 = floor(ordered_corners[2][0]);
	data.y1 = floor(ordered_corners[0][1]);
	data.y2 = floor(ordered_corners[1][1]);
	data.y3 = floor(ordered_corners[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	raster_upper(app, vtc, triangle, &data);
	raster_lower(app, vtc, triangle, &data);

	/*
	** sort vertices in height order in an array
	** find y values for every x value for lines {0;1}, {1;2}, {0;2}
	** scan horrizontal lines from point x,y on lines {0;1} and {1;2}
	** to x,y point on line {0;2}
	** using formula a to b
	** a += (b-a)/(abs()b-a) until a == b
	*/
	(void)camera;
}

void	draw_debug_crosshair_on_corners(t_wolf3d *app, t_vec2 *ordered_corners)
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

t_bool			render_triangle(t_wolf3d *app, t_triangle *triangle)
{
	t_vec2				corners_on_screen[3];
	t_vec2				ordered_corners[3];
	t_vec2				corners[3];
	t_vec3				dir;
	int					i;

	if (triangle_behind_camera(triangle, app->active_scene->main_camera))
		return (false);
	ml_vector3_sub(triangle->center, app->active_scene->main_camera->origin, dir);
	if (!is_triangle_facing(triangle, dir))
		return (false);
	screen_intersection(app->active_scene->main_camera, triangle,
		corners_on_screen);
	rasterize_triangle(app, triangle, ordered_corners, corners_on_screen,
						app->active_scene->main_camera);
	i = -1;
	while (++i < 3)
	{
		corners[i][0] = corners_on_screen[i][0] + app->main_window->width / 2;
		corners[i][1] = corners_on_screen[i][1] + app->main_window->height / 2;
		ordered_corners[i][0] += app->main_window->width / 2;
		ordered_corners[i][1] += app->main_window->height / 2;
	}
	// l3d_triangle_2d_draw(app->main_window->rbuffer,
	// 	(uint32_t[2]){app->main_window->width,
	// 	app->main_window->height},
	// 	corners, app->main_window->rbuf_render_color / 5);
	// draw_debug_crosshair_on_corners(app, ordered_corners);
	i = -1;
	while (++i < app->main_window->width * app->main_window->height)
		app->main_window->framebuffer[i] = app->main_window->rbuffer[i];
	return (true);
}

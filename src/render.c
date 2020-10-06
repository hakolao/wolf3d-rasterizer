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
#define BREAKLIMIT 1000//!debug only

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

/*
**	Creates triangles to represent the screen. First triangle is top left, 
**	second is bottom right.
*/

// void		create_screen_triangles(t_wolf3d *app, t_vec3 position,
// 									float *dimensions, t_triangle *result)
// {
// 	result[0].vtc = 
// }

// typedef struct s_hit
// {
// 	float t;
// 	float u;
// 	float v;
// 	t_vec3 normal;
// 	t_vec3 hit_point;
// }			t_hit;

void		calculate_2d_points(t_vec2 *points_2d, t_vec3 *hits, t_ray *rays)
{
	int		i;
	t_vec3	dir[3];
	i = -1;
	while (++i < 3)
	{
		// ml_vector3_normalize(rays[i].dir, dir[i]);
		// ml_vector3_mul(dir[i], hits[i].t, dir[i]);//??
		points_2d[i][0] = hits[i][0];
		points_2d[i][1] = hits[i][1];
		(void)rays;
		(void)dir;
	}
}

typedef struct	s_plane
{
	t_vec3		origin;
	t_vec3		normal;
}				t_plane;

t_bool		screen_ray_intersect(t_plane *plane, t_ray *ray, t_vec3 hit_point)
{
	t_vec3	temp;
	float	div;
	float	d;
	ft_printf("ray intersect with plane: \n");
	ml_vector3_print(ray->dir);
	ml_vector3_sub(plane->origin, ray->origin, temp);
	if (fabs((div = ml_vector3_dot(ray->dir, plane->normal))) > L3D_EPSILON)
	{
		d = (ml_vector3_dot(temp, plane->normal)) / div;
		ml_vector3_mul(ray->dir, d, hit_point);
		ml_vector3_add(temp, ray->origin, hit_point);
		return (true);
	}
	return (false);
}

t_bool		screen_intersection(t_wolf3d *app, t_camera *camera,
								t_triangle *triangle, t_vec2 *corners_on_screen)
{
	// t_ray	rays[3];
	t_ray	trays[3];	
	t_vec2	points_2d[3];
	t_vec3	hits[3];
	int k = -1;
	int		i;
	// float	scaler;
	i = -1;
	// while (++i < 3)
	// {
	// 	l3d_ray_set(triangle->vtc[i]->pos, (t_vec3){0.0, 0.0, 0.0}, &rays[i]);
	// 	scaler = (camera->screen_dist / rays[i].dir[2]);
	// 	rays[i].dir[0] *= scaler;
	// 	rays[i].dir[1] *= scaler;
	// 	ml_vector2_copy((t_vec2){rays[i].dir[0], rays[i].dir[1]},
	// 		corners_on_screen[i]);
	// }
	t_plane screen;
	ml_vector3_set(screen.origin, 0.0, 0.0,
					-app->active_scene->main_window->width);
	ml_vector3_set(screen.normal, 0.0, 0.0, 1.0);
	while (++k < 3)
	{
		//ft_printf("here2\n");
		l3d_ray_set(triangle->vtc[k]->pos, (t_vec3){0.0, 0.0, 0.0}, &trays[k]);
		if (!(screen_ray_intersect(&screen, &trays[k], hits[k])))
			{
				ft_printf("Error in screen_intersection: ray from triangle\n"
							"didn't intersect with screen plane or did in too\n"
							"small an angle. Pointo of hit set to default 0.\n");
				ml_vector3_set_all(hits[k], 0);
			}
			ft_printf("screen intersection point: \n");
			ml_vector3_print(hits[k]);
	}
	calculate_2d_points(points_2d, hits, trays);
	int j = -1;
	while (++j < 3)
	{
		//ft_printf("here1\n");
		corners_on_screen[j][0] = points_2d[j][0];
		corners_on_screen[j][1] = points_2d[j][1];
	}
	//ft_printf("got out\n");
	return (true);
	(void)trays;
	(void)hits;
	(void)points_2d;
	(void)camera;
	(void)corners_on_screen;
	(void)i;
	(void)k;
	(void)triangle;
	(void)app;
}

void		order_corners_y(t_triangle *triangle, t_vertex **vtc, t_vec2 *ordered_corners, t_vec2 *corners_on_screen)
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

void		scan_line(t_wolf3d *app, float *limits, t_vertex **vtc, t_triangle *triangle)
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
			// ft_printf("break2\n");
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

void		raster_upper(t_wolf3d *app, t_vertex **vtc, t_triangle *triangle, t_raster_data *data)
{
	float x;
	float y;
	float end_x;

	int i = 0;

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
		i++;
		if (i > BREAKLIMIT)
		{
			//ft_printf("brokeupper\n");
			break;
		}
	}
	
}

void		raster_lower(t_wolf3d *app, t_vertex **vtc, t_triangle *triangle, t_raster_data *data)
{
	float x;
	float y;
	float end_x;

	int i = 0;

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
		i++;
		if (i > BREAKLIMIT)
		{
			//ft_printf("brokelower\n");
			break;
		}
	}
}

void		rasterize_triangle(t_wolf3d *app, t_triangle *triangle, t_vec2 *ordered_corners,
							t_vec2 *corners_on_screen, t_camera *camera)
{
	t_vertex *vtc[3];
	t_raster_data data;

	order_corners_y(triangle, vtc, ordered_corners, corners_on_screen);
	//ft_printf("ordered\n");
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
	//ft_printf("rastered upper\n");
	raster_lower(app, vtc, triangle, &data);
	//ft_printf("rastered lower\n");

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
	t_vec2		corners_on_screen[3];
	t_vec2		ordered_corners[3];
	int			i;

	if (!(is_rendered(app, triangle)))
		return (false);
	screen_intersection(app, app->active_scene->main_camera, triangle,
		corners_on_screen);
	rasterize_triangle(app, triangle, ordered_corners, corners_on_screen,
						app->active_scene->main_camera);//?expose this
	// draw_debug_crosshair_on_corners(app, ordered_corners);
	i = -1;
	while (++i < app->main_window->width * app->main_window->height)
		app->main_window->framebuffer[i] = app->main_window->rbuffer[i];
	return (true);
}

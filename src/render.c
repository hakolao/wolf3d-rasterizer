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


void		fill_triangle_area_column(t_wolf3d *app, uint32_t *zbuffer, int startx, int starty)
{
	static int counter = 0;
	// ft_printf("counter: %d\n", counter);
	counter++;
	uint32_t		width;
	uint32_t height;
	uint32_t	index;

	width = app->main_window->width;
	height = app->main_window->height;
	index = screen_to_frame_coords(width, height, startx + width / 2, starty + height / 2);
	// ft_printf("startx: %d | starty: %d", startx, starty);
	// ft_printf("index: %d\n", index);
	// // if (counter > 1427)
	// // 	return ;
	// ft_printf("hex: %x\n", zbuffer[index]);
	if (zbuffer[index] == 0xffaaffff)
	{
		return ;
	}
	else if (index > 0 && index < width * height - 1)
	{
		zbuffer[index] = 0xffaaffff;
			fill_triangle_area_column(app, zbuffer, startx, starty + 1);
			fill_triangle_area_column(app, zbuffer, startx, starty - 1);
			fill_triangle_area_column(app, zbuffer, startx + 1, starty);
			fill_triangle_area_column(app, zbuffer, startx - 1, starty);
	}
}

void		vec2_sub_y_z(t_vec2 v1, t_vec2 v2, t_vec2 res)
{
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
}

void		paint_edge_x(t_wolf3d *app, t_vec2 *start, t_vec2 edge_vector, float deltax)
{
	float	increment;
	int		i;

	i = -1;
	while (++i < ft_abs((double)deltax))
	{
		increment = i / (ft_abs((double)deltax));
		app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width,
									   app->main_window->height,
									   (int)((*start)[0] + increment * edge_vector[0] +
											 app->main_window->width / 2),
									   (int)((*start)[1] + increment * edge_vector[1] +
											 app->main_window->height / 2))] = 0xffaaffff;
		i++;
	}
}

void		paint_edge_y(t_wolf3d *app, t_vec2 *start, t_vec2 edge_vector, float deltay)
{
	float	increment;
	int		i;

	i = -1;
	while (++i < ft_abs((double)deltay))
	{
		increment = i / (ft_abs((double)deltay));
		app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width,
									   app->main_window->height,
									   (int)((*start)[0] + increment * edge_vector[0] +
											 app->main_window->width / 2),
									   (int)((*start)[1] + increment * edge_vector[1] +
											 app->main_window->height / 2))] = 0xffaaffff;
		i++;
	}
}

void		paint_edge_to_buffer(t_wolf3d *app, t_vec2 *start, t_vec2 *end)
{
	float		delta[2];
	int			i;
	t_vec2		edge_vector;
	float		edge_vector_magn;
	i = 0;
	delta[0] = ((float)((*end)[0] - (*start)[0])) * 2;
	delta[1] = ((float)((*end)[1] - (*start)[1])) * 2;
	vec2_sub_y_z(*end, *start, edge_vector);
	edge_vector_magn = ml_vector2_mag(edge_vector);
	paint_edge_x(app, start, edge_vector, delta[0]);
	paint_edge_y(app, start, edge_vector, delta[1]);
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
			t_mesh *mesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	int					i;
	t_vec2				corners_on_screen[3];
	// t_vec3				normalized_normal;
	uint32_t			*rbuffer;
	int					width = app->main_window->width;
	int					height = app->main_window->height;
	(void)width;
	(void)height;

	rbuffer = app->main_window->rbuffer;
	(void)mesh;
	screen_intersection(app, triangle, corners_on_screen);
	paint_edge_to_buffer(app, &(corners_on_screen[0]), &(corners_on_screen[1]));
	paint_edge_to_buffer(app, &(corners_on_screen[0]), &(corners_on_screen[2]));
	paint_edge_to_buffer(app, &(corners_on_screen[1]), &(corners_on_screen[2]));
	fill_triangle_area_column(app, rbuffer, 0, 0);
	// printf("fill triangle startx: %d\n", (int)((corners_on_screen[0])[0]) - 1);
	// printf("fill triangle starty: %d\n", (int)((corners_on_screen[0])[1]));
	//TODO WE NOW HAVE A FUNCTION THAT PAINTS AN ENTIRE PIXEL COLUMN WITHIN BOUNDARIES
	//TODO NEED TO IMPLEMENT A LOOP THAT RUNS IT FOR EVERY COLUMN IN TRIANGLE
	//TODO REMEMBER TO CHANGE TRIANGLE AND GRID RENDERING BACK
	int k = 0;
	while (k < width * height)
	{
		app->main_window->framebuffer[k] = rbuffer[k];
		k++;
	}
	ft_memset(rbuffer, 0, sizeof(float) * WIDTH * HEIGHT);
	// ml_vector3_normalize(triangle->normal, normalized_normal);
	color = 0xffaaffff;
	//Here we borrow zbuffers memory to store the triangle's screen coordinates
	//the area will be filled right after with triangles z values (might be fragile af)
	// zbuffer[screen_to_frame_coords(width, height, + width / 2, + height / 2)] = 0;
	// paint_edge_to_buffer(uint32_t *zbuffer, t_vec2 *start, t_vec2 *end);,
	//1. Cast rays from triangle corners through screen to camera
	//2. Calculate intersection points with screen and convert them to
	//		screen pixels.
	//3. Calculate border edges of the triangle in screen pixels using triangle
	//		elemental vectors and the difference in x value between ab and ac.
	//4. Use recursive filling algorithm to find out the pixel coordinates
	//		inside the triangle.
	//5. Use all these pixels which are now guaranteed to be inside the triangle
	//		and cast rays from camera through the pixels and calculate intersection
	//		normally with the triangle.
	i = 0;
	while (i < camera->raycount)
	{
		// if (triangle_intersection(triangle, &(camera->rays[i]), &intsec))
		// {
		// 	app->main_window->framebuffer[
		// 		screen_to_frame_coords(app->main_window->width, app->main_window->height,
		// 		(int)(camera->rays[i].dir[1]) + camera->screen_width / 2,
		// 		(int)(camera->rays[i].dir[2]) + camera->screen_height / 2)] = color;
		// }
		i++;
		//find a way to get uv data in fragment shader
		//mesh->shader->f(triangle, calculate_baryocoords(intsec), color);
		//find a way to get framebuffer data in and or out of this function
	}
	(void)intsec;
	(void)corners_on_screen;
	(void)triangle;
	return (true);
}

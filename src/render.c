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
#define FILL_ANIMATION 1
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

void		fill_triangle_corners(t_wolf3d *app, int *ints_on_screen, int *triangle_center)
{
	int		i;
	int		deltax;
	int		deltay;
	//!MAKE THIS FUNCTION WORK SO THAT IT FILLS THE END POINTS CORRECTLY> THEN BORDERS WILL HOPEFULLY WORK
	i = -1;
	deltax = 0;
	deltay = 0;
	while (++i < 3)
	{
		deltax = ints_on_screen[i * 2] - triangle_center[0];
		deltay = ints_on_screen[i * 2 + 1] - triangle_center[1];
		if (fabs((double)(deltax > deltay)))
			{
				// ft_printf("here1\n");
				if (deltax > 0)
				{
					// ft_printf("deltax > 0\n");
					app->main_window->rbuffer[
						screen_to_frame_coords(app->main_window->width, app->main_window->height,
						ints_on_screen[i * 2] + 1, ints_on_screen[i * 2 + 1])] =
						app->main_window->rbuf_render_color / 2;
				}
				else
				{
					// ft_printf("deltax <= 0\n");
					app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width, app->main_window->height,
																	 ints_on_screen[i * 2] - 1, ints_on_screen[i * 2 + 1])] =
						app->main_window->rbuf_render_color / 2;
				}
			}
			else
			{
				if (deltay > 0)
				{
					// ft_printf("deltay > 0\n");
					app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width, app->main_window->height,
																	 ints_on_screen[i * 2], ints_on_screen[i * 2 + 1] + 1)] =
						app->main_window->rbuf_render_color / 2;
				}
				else
				{
					// ft_printf("deltay <= 0\n");
					app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width, app->main_window->height,
																	 ints_on_screen[i * 2], ints_on_screen[i * 2 + 1] - 1)] =
						app->main_window->rbuf_render_color / 2;
				}
			}
	}
}

/*
**	Flood fills the triangle area in rbuffer. This information is used when
**	casting rays. 
*/

void		fill_triangle_area(t_wolf3d *app, t_vertex *vtx, int *start)
{
	static int counter = 0;
	uint32_t		width;
	uint32_t		height;
	uint32_t		index;
	uint32_t		*rbuffer;

	counter++;
	rbuffer = app->main_window->rbuffer;
	width = app->main_window->width;
	height = app->main_window->height;
	index = screen_to_frame_coords(width, height, start[0] + width / 2, start[1] + height / 2);
	if (index > 0 && index < width * height - 1 &&
		(
		 rbuffer[index] == app->main_window->rbuf_render_color / 2))
		{
			// ft_printf("returned\n");
			return;
		}
	if (index > 0 && index < width * height - 1)
	{
		rbuffer[index] = app->main_window->rbuf_render_color / 2;
		//////////!
		uint32_t k = 0;
		if (FILL_ANIMATION)
		{
			if (counter % FILL_SPEED == 0)
			{SDL_LockTexture(app->main_window->frame, NULL,
							(void **)&app->main_window->framebuffer,
							&app->main_window->pitch);
			while (k < width * height)
			{
				app->main_window->framebuffer[k] = rbuffer[k];
				k++;
			}
			SDL_UnlockTexture(app->main_window->frame);
			SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
						NULL, NULL);
			SDL_RenderPresent(app->main_window->renderer);
			SDL_Delay(1);}
		}
		//////////!

	
		fill_triangle_area(app, vtx, (int[2]){start[0], start[1] - 1});
		fill_triangle_area(app, vtx, (int[2]){start[0], start[1] + 1});
		fill_triangle_area(app, vtx, (int[2]){start[0] - 1, start[1]});
		fill_triangle_area(app, vtx, (int[2]){start[0] + 1, start[1]});
	}
}

void		vec2_sub_y_z(t_vec2 v1, t_vec2 v2, t_vec2 res)
{
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
}

void		calculate_triangle_center(t_triangle *triangle,
										int *triangle_center)
{
	triangle_center[0] = (int)((triangle->vtc[0]->position[1] +
						triangle->vtc[1]->position[1] +
						triangle->vtc[2]->position[1]) / 3);
						printf("calc center x: %d\n", triangle_center[0]);
	triangle_center[1] = (int)((triangle->vtc[0]->position[2] +
								triangle->vtc[1]->position[2] +
								triangle->vtc[2]->position[2]) /
							   3);
						printf("calc center y: %d\n", triangle_center[1]);
}

void		round_start_points(t_vertex *vtx, int *start_points, t_vec2 center_dir, t_vec2 travelled_dist)
{
	if (center_dir[0] < 0)
	{
		start_points[0] = ft_floor(vtx->position[1] + travelled_dist[0]);
		ft_printf("here1\n");
	}
	else
	{
		start_points[0] = ft_ceil(vtx->position[1] + travelled_dist[0]);
		ft_printf("here2\n");
	}
	if (center_dir[1] < 0)
	{
		start_points[1] = ft_floor(vtx->position[2] + travelled_dist[1]);
		ft_printf("here3\n");
	}
	else
	{
		start_points[1] = ft_ceil(vtx->position[2] + travelled_dist[1]);
		ft_printf("here4\n");
	}
	ft_printf("start points: %d | %d\n", start_points[0], start_points[1]);
}

/*
**	Calculates the valid starting points for filling the triangle for each
**	corner. 
*/

void		calculate_fill_start_points(t_wolf3d *app, t_triangle *triangle,
										int *triangle_center, int *start_points)
{
	t_vec2			center_dir[3];
	float			i;
	int				k;
	t_vec2			travelled_dist;

	i = 0.0;
	k = -1;
	while (++k < 3)
	{
		center_dir[k][0] = triangle_center[0] - triangle->vtc[k]->position[1];
		center_dir[k][1] = triangle_center[1] - triangle->vtc[k]->position[2];
		triangle_center[2 + k * 2] = center_dir[k][0];
		triangle_center[2 + k * 2 + 1] = center_dir[k][1];
		ml_vector2_normalize(center_dir[k], center_dir[k]);
	}
	k = -1;
	while (++k < 3)
	{
		ft_memset(travelled_dist, 0, sizeof(travelled_dist));
		ft_printf("loop2 K: %d\n", k);
		ft_memset(travelled_dist, 0, sizeof(travelled_dist));
		ft_printf("triangle->vtc[k]->position[1]: %f\n", triangle->vtc[k]->position[1]);
		printf("triangle->vtc[k]->position[2]: %f\n", triangle->vtc[k]->position[2]);

		i = (uint32_t)screen_to_frame_coords(app->main_window->width, app->main_window->height,
				triangle->vtc[k]->position[1] + travelled_dist[0] + app->main_window->width / 2,
				triangle->vtc[k]->position[2] + travelled_dist[1] + app->main_window->height / 2);
		ft_printf("i: %u\n", i);
		ft_printf("start pixel color before loop3: %x\n",
				  app->main_window->rbuffer[(int)i]);
		start_points[2 * k] = -1 * ((int)i % app->main_window->width - app->main_window->width / 2);
		start_points[2 * k + 1] = -1 * ((int)(i / app->main_window->width) - app->main_window->height / 2);
		ft_printf("start points before loop3, %d | %d\n", (start_points + k * 2)[0], (start_points + k * 2)[1]);
		while (app->main_window->rbuffer[(int)i] == app->main_window->rbuf_render_color )//&&
				//ml_vector2_mag(travelled_dist) < delta_center[k] * 2)
		{
			ft_printf("loop3\n");
			travelled_dist[0] += center_dir[k][0];
			travelled_dist[1] += center_dir[k][1];
			ft_printf("travelled_dist[0]: %f\n", travelled_dist[0]);
			ft_printf("travelled_dist[1]: %f\n", travelled_dist[1]);
			i = screen_to_frame_coords(app->main_window->width, app->main_window->height,
				triangle->vtc[k]->position[1] + travelled_dist[0] + app->main_window->width / 2,
				triangle->vtc[k]->position[2] + travelled_dist[1] + app->main_window->height / 2);
			// ft_printf("triangle->vtc[k]->position[1]: %f\n", triangle->vtc[k]->position[1]);
			// ft_printf("digit: %u\n", triangle->vtc[k]->position[1]);
			// ft_printf("triangle->vtc[k]->position[2]: %f\n", triangle->vtc[k]->position[2]);
			ft_printf("i3: %u\n", i);
			ft_printf("start Y for k: %f %d\n", triangle->vtc[k]->position[1] + travelled_dist[0], k);
			ft_printf("start Z for k: %f %d\n", triangle->vtc[k]->position[2] + travelled_dist[1], k);
			// ft_printf("start pixel color: %x\n", app->main_window->rbuffer[i]);
		}
		round_start_points(triangle->vtc[k], start_points + (2 * k), center_dir[k], travelled_dist);
		ft_printf("start color: %x: \n", app->main_window->rbuffer[screen_to_frame_coords(app->main_window->width, app->main_window->height,
				start_points[0] + app->main_window->width / 2,
				start_points[1] + app->main_window->height / 2)]);
		// start_points[2 * k] = ft_abs_ceil(triangle->vtc[k]->position[1] + travelled_dist[0]);
		// start_points[1 + 2 * k] = ft_abs_ceil(triangle->vtc[k]->position[2] + travelled_dist[1]);
		ft_printf("===================================\n");
	}
	
}


void		draw_triangle_edges(t_wolf3d *app, int *ints_on_screen, uint32_t color)
{
	if (ints_on_screen[0] > ints_on_screen [2])
		draw_line_lower((t_ir){.a = ints_on_screen[0] + WIDTH / 2, .b = ints_on_screen[1] + HEIGHT / 2},
						(t_ir){.a = ints_on_screen[2] + WIDTH / 2, .b = ints_on_screen[3] + HEIGHT / 2}, color, app);
	else
		draw_line_upper((t_ir){.a = ints_on_screen[0] + WIDTH / 2, .b = ints_on_screen[1] + HEIGHT / 2},
						(t_ir){.a = ints_on_screen[2] + WIDTH / 2, .b = ints_on_screen[3] + HEIGHT / 2}, color, app);
	if (ints_on_screen[0] > ints_on_screen[4])
		draw_line_upper((t_ir){.a = ints_on_screen[0] + WIDTH / 2, .b = ints_on_screen[1] + HEIGHT / 2},
						(t_ir){.a = ints_on_screen[4] + WIDTH / 2, .b = ints_on_screen[5] + HEIGHT / 2}, color, app);
	else
		draw_line_lower((t_ir){.a = ints_on_screen[0] + WIDTH / 2, .b = ints_on_screen[1] + HEIGHT / 2},
						(t_ir){.a = ints_on_screen[4] + WIDTH / 2, .b = ints_on_screen[5] + HEIGHT / 2}, color, app);
	if (ints_on_screen[4] > ints_on_screen [2])
		draw_line_upper((t_ir){.a = ints_on_screen[4] + WIDTH / 2, .b = ints_on_screen[5] + HEIGHT / 2},
						(t_ir){.a = ints_on_screen[2] + WIDTH / 2, .b = ints_on_screen[3] + HEIGHT / 2}, color, app);
	else
		draw_line_lower((t_ir){.a = ints_on_screen[4] + WIDTH / 2, .b = ints_on_screen[5] + HEIGHT / 2},
						(t_ir){.a = ints_on_screen[2] + WIDTH / 2, .b = ints_on_screen[3] + HEIGHT / 2}, color, app);
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
	int					triangle_center[8];
	// int					start_points[6];
	int					ints_on_screen[6];

	// app->main_window->rbuf_render_color = 0xffaaffff / 2;
	rbuffer = app->main_window->rbuffer;
	(void)mesh;
	screen_intersection(app, triangle, corners_on_screen);
	int j = -1;
	while (++j < 3)
	{
		ints_on_screen[j * 2] = (int)corners_on_screen[j][0];
		ints_on_screen[j * 2 + 1] = (int)corners_on_screen[j][1];
	}
	uint32_t linecolor = 0xffaaffff;
	//!DRAW ORDER: AB, BC, CA
	draw_triangle_edges(app, ints_on_screen, linecolor);
	calculate_triangle_center(triangle, triangle_center);
	// printf("fill triangle startx: %d\n", triangle_center[0]);
	// printf("fill triangle starty: %d\n", triangle_center[1]);
	// calculate_fill_start_points(app, triangle, triangle_center, start_points);
	// ft_printf("output start points A: %d | %d\n", start_points[0], start_points[1]);
	// ft_printf("output start points B: %d | %d\n", start_points[2], start_points[3]);
	// ft_printf("output start points C: %d | %d\n", start_points[4], start_points[5]);
	// ft_printf("start pixel color: %x\n",
	// 		  app->main_window->rbuffer[screen_to_frame_coords(width, height, start_points[0]
	// 		  + width / 2, start_points[1] + height / 2)]);
	fill_triangle_corners(app, ints_on_screen, triangle_center);
	fill_triangle_area(app, triangle->vtc[0], triangle_center);
	//TODO OFFSET THE TRIANGLE CENTER BY CORRECT AMOUNT DEPENDING OF WHICH CORNER IS START
	

	//TODO REMEMBER TO CHANGE TRIANGLE AND GRID RENDERING BACK
	int k = 0;
	while (k < width * height)
	{
		app->main_window->framebuffer[k] = rbuffer[k];
		k++;
	}
	ft_memset(rbuffer, 0, sizeof(float) * WIDTH * HEIGHT);
	if (RANDOM_COLOR)
		app->main_window->rbuf_render_color = sin(SDL_GetTicks() / 200) * 0xffffffff;
	// ml_vector3_normalize(triangle->normal, normalized_normal);
	color = app->main_window->rbuf_render_color;
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
	(void)width;
	(void)height;
	(void)camera;
	// while (i < camera->raycount)
	// {
	// 	// if (triangle_intersection(triangle, &(camera->rays[i]), &intsec))
	// 	// {
	// 	// 	app->main_window->framebuffer[
	// 	// 		screen_to_frame_coords(app->main_window->width, app->main_window->height,
	// 	// 		(int)(camera->rays[i].dir[1]) + camera->screen_width / 2,
	// 	// 		(int)(camera->rays[i].dir[2]) + camera->screen_height / 2)] = color;
	// 	// }
	// 	i++;
	// 	//find a way to get uv data in fragment shader
	// 	//mesh->shader->f(triangle, calculate_baryocoords(intsec), color);
	// 	//find a way to get framebuffer data in and or out of this function
	// }
	(void)intsec;
	(void)corners_on_screen;
	(void)triangle;
	return (true);
}

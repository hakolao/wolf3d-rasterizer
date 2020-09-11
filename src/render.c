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
#define FILL_SPEED 200
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

/*
**	Flood fills the triangle area in rbuffer. This information is used when
**	casting rays. 
*/

void		fill_triangle_area(t_wolf3d *app, int startx, int starty)
{
	static int counter = 0;
	uint32_t		width;
	uint32_t		height;
	uint32_t		index;
	// uint32_t		adjacent;
	uint32_t		*rbuffer;
	counter++;
	// adjacent = 0;
	rbuffer = app->main_window->rbuffer;
	width = app->main_window->width;
	height = app->main_window->height;
	index = screen_to_frame_coords(width, height, startx + width / 2, starty + height / 2);
	if (index > 0 && index < width * height - 1 &&
		(rbuffer[index] == app->main_window->rbuf_render_color ||
		 rbuffer[index] == app->main_window->rbuf_render_color / 2))
		{
			// ft_printf("returned\n");
			return;
		}
		// ft_printf("not returned\n");
		 //!TODO CHANGE BOUNDARY COLOR TO SOMETHING SENSIBLE
	if (index > 0 && index < width * height - 1)
	{
		// adjacent = screen_to_frame_coords(width, height, startx + width / 2 + 1, starty + height / 2);
		// if (adjacent > 0 && adjacent < width * height - 1 && rbuffer[adjacent]
		// 	== app->main_window->rbuf_render_color - 1)
		// 	{
		// 		fill_triangle_area(app, startx + 1, starty);
		// 	}
		// ft_printf("index: %d\n", index);
		rbuffer[index] = app->main_window->rbuf_render_color;
		//////////!
		SDL_Event event;
		uint32_t k = 0;
		if (FILL_ANIMATION)
		{
			while (SDL_PollEvent(&event))
		{
			player_action_handle(app, event);
		}
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
		fill_triangle_area(app, startx, starty + 1);
		fill_triangle_area(app, startx, starty - 1);
		fill_triangle_area(app, startx + 1, starty);
		fill_triangle_area(app, startx - 1, starty);
	}
	
}

void		vec2_sub_y_z(t_vec2 v1, t_vec2 v2, t_vec2 res)
{
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
}

/*
**	Paints the edges of the triangle into rbuffer keeping horizontal borders
**	contiguous.
*/

void		paint_edge_x(t_wolf3d *app, t_vec2 *start, t_vec2 edge_vector, float deltax)
{
	float	increment;
	int		index;
	int		i;
	uint32_t *rbuffer =  app->main_window->rbuffer;
	i = -1;
	index = 0.0;
	while (++i < ft_abs((double)deltax))
	{
		increment = i / (ft_abs((double)deltax));
		index = screen_to_frame_coords(app->main_window->width,
			app->main_window->height,
			(int)((*start)[0] + increment * edge_vector[0] +
				app->main_window->width / 2),
			(int)((*start)[1] + increment * edge_vector[1] +
				app->main_window->height / 2));
		if (index >= 0 && index < app->main_window->width * app->main_window->height)
			app->main_window->rbuffer[index] = app->main_window->rbuf_render_color;
		i++;
		////////////!
		uint32_t k = 0;
		if (ANIMATION)
		{
			SDL_LockTexture(app->main_window->frame, NULL,
							(void **)&app->main_window->framebuffer,
							&app->main_window->pitch);
			while (k < WIDTH * HEIGHT)
			{
				app->main_window->framebuffer[k] = rbuffer[k];
				k++;
			}
			SDL_UnlockTexture(app->main_window->frame);
			SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
						   NULL, NULL);
			SDL_RenderPresent(app->main_window->renderer);
			SDL_Delay(EDGE_DELAY);
		}
		//////////////!
	}
}

/*
**	Paints the edges of the triangle into rbuffer keeping vertical borders
**	contiguous.
*/

void		paint_edge_y(t_wolf3d *app, t_vec2 *start, t_vec2 edge_vector, float deltay)
{
	float	increment;
	int		index;
	int		i;
	uint32_t *rbuffer = app->main_window->rbuffer;

	i = -1;
	index = 0;
	while (++i < ft_abs((double)deltay))
	{
		increment = i / (ft_abs((double)deltay));
		index = screen_to_frame_coords(app->main_window->width,
			app->main_window->height,
			(int)((*start)[0] + increment * edge_vector[0] +
				app->main_window->width / 2),
			(int)((*start)[1] + increment * edge_vector[1] +
				app->main_window->height / 2));
		if (index >= 0 && index < app->main_window->width * app->main_window->height)
			app->main_window->rbuffer[index] = app->main_window->rbuf_render_color;
		i++;
		////////////!
		uint32_t k = 0;
		if (ANIMATION)
		{
			app->main_window->rbuffer[index] = app->main_window->rbuf_render_color / 2;
			SDL_LockTexture(app->main_window->frame, NULL,
							(void **)&app->main_window->framebuffer,
							&app->main_window->pitch);
			while (k < WIDTH * HEIGHT)
			{
				app->main_window->framebuffer[k] = rbuffer[k];
				k++;
			}
			SDL_UnlockTexture(app->main_window->frame);
			SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
						   NULL, NULL);
			SDL_RenderPresent(app->main_window->renderer);
			SDL_Delay(EDGE_DELAY);
		}
		//////////////!
	}
}

/*
**	Paints the triangle edges into rbuffer to be flood filled
*/

void		paint_edge_to_rbuffer(t_wolf3d *app, t_vec2 *start, t_vec2 *end)
{
	float		delta[2];
	int			i;
	t_vec2		edge_vector;
	float		edge_vector_magn;
	i = 0;
	delta[0] = ((float)((*end)[0] - (*start)[0])) * 3;//TODO THIS MULTIPLIER INCREASES ACCURACY
	delta[1] = ((float)((*end)[1] - (*start)[1])) * 3;//TODO MUST TEST FOR PERFORMANCE, 3 SEEMS
	vec2_sub_y_z(*end, *start, edge_vector);		  //TODO TO BE ACCURATE ENOUGH
	edge_vector_magn = ml_vector2_mag(edge_vector);
	paint_edge_x(app, start, edge_vector, delta[0]);
	paint_edge_y(app, start, edge_vector, delta[1]);
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

/*
**	Calculates the valid starting points for filling the triangle for each
**	corner. 
*/

void		calculate_fill_start_points(t_wolf3d *app, t_triangle *triangle,
										int *triangle_center, int *start_points)
{
	t_vec2			center_dir[3];
	float			delta_center[3];
	uint32_t		i;
	int				k;
	t_vec2			travelled_dist;

	i = 0;
	k = -1;
	while (i < 3)
	{
		center_dir[i][0] = triangle_center[0] - triangle->vtc[i]->position[1];
		center_dir[i][1] = triangle_center[1] - triangle->vtc[i]->position[2];
		ml_vector2_normalize(center_dir[i], center_dir[i]);
		delta_center[i] = ml_vector2_mag((t_vec2){center_dir[i][0], center_dir[i][1]});
		i++;
	}
	while (++k < 3)
	{
		ft_printf("loop2\n");
		ft_memset(travelled_dist, 0, sizeof(travelled_dist));
		ft_printf("triangle->vtc[k]->position[1]: %f\n", triangle->vtc[k]->position[1]);
		ft_printf("triangle->vtc[k]->position[2]: %f\n", triangle->vtc[k]->position[2]);
		i = (uint32_t)screen_to_frame_coords(app->main_window->width, app->main_window->height,
								triangle->vtc[k]->position[1] + app->main_window->width / 2,
								triangle->vtc[k]->position[2] + app->main_window->height / 2);
		ft_printf("i: %u\n", i);
		while (app->main_window->rbuffer[i] == app->main_window->rbuf_render_color &&
				ml_vector2_mag(travelled_dist) < delta_center[k] + 1)
		{
			ft_printf("loop3\n");
			travelled_dist[0] += center_dir[k][0];
			travelled_dist[1] += center_dir[k][1];
			ft_printf("travelled_dist[0]: %f\n", travelled_dist[0]);
			ft_printf("travelled_dist[1]: %f\n", travelled_dist[1]);
			i = screen_to_frame_coords(app->main_window->width, app->main_window->height,
			triangle->vtc[k]->position[1] + app->main_window->width / 2 + travelled_dist[0],
				triangle->vtc[k]->position[2] + app->main_window->height / 2 + travelled_dist[1]);
			ft_printf("triangle->vtc[k]->position[1]: %f\n", triangle->vtc[k]->position[1]);
			ft_printf("triangle->vtc[k]->position[2]: %f\n", triangle->vtc[k]->position[2]);
			ft_printf("i3: %u\n", i);
			start_points[0 + k] = triangle->vtc[k]->position[1] + app->main_window->width / 2 + travelled_dist[0];
			start_points[1 + 2 * k] = triangle->vtc[k]->position[2] + app->main_window->height / 2 + travelled_dist[1];
		}
	}
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
	int					triangle_center[2];
	int					start_points[6];
	(void)width;
	(void)height;
	(void)camera;

	rbuffer = app->main_window->rbuffer;
	(void)mesh;
	screen_intersection(app, triangle, corners_on_screen);
	paint_edge_to_rbuffer(app, &(corners_on_screen[0]), &(corners_on_screen[1]));
	paint_edge_to_rbuffer(app, &(corners_on_screen[0]), &(corners_on_screen[2]));
	paint_edge_to_rbuffer(app, &(corners_on_screen[1]), &(corners_on_screen[2]));
	// int x = (int)triangle->vtc[0]->position[1];
	// int y = (int)triangle->vtc[0]->position[2];
	calculate_triangle_center(triangle, triangle_center);
	// printf("fill triangle startx: %d\n", triangle_center[0]);
	// printf("fill triangle starty: %d\n", triangle_center[1]);
	calculate_fill_start_points(app, triangle, triangle_center, start_points);
	ft_printf("output start points: %d | %d", start_points[4], start_points[5]);
	// fill_triangle_area(app, start_points[4], start_points[5] - 5);

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

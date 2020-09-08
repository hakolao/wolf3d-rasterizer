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
		ml_vector3_normalize(rays[i].dir, rays[i].normalized_dir);
		scaler = (app->active_scene->main_camera->screen_dist / rays[i].dir[0]);
		rays[i].dir[1] *= scaler;
		rays[i].dir[2] *= scaler;
		ml_vector2_copy((t_vec2){rays[i].dir[1], rays[i].dir[2]},
						corners_on_screen[i]);
	}
}

// void		fill_triangle_area(uint32_t *zbuffer, int startpoint)
// {

// }

void		paint_edge_to_buffer(t_wolf3d *app, uint32_t *zbuffer, t_vec2 *start, t_vec2 *end)
{
	int			increment;
	int			deltax;
	int			i;
	t_vec2		edge_vector;
	float		edge_vector_magn;
	// int			xi;
	// int			yi;

	i = 0;
	// xi = 0;
	// yi = 0;
	// xi = (*start)[0] + increment * edge_vector[0];
	// yi = (*start)[1] + increment * edge_vector[1];
	deltax = (*end)[0] - (*start)[0];
	edge_vector_magn = ml_vector2_mag(edge_vector);
	ml_vector2_sub(*end, *start, edge_vector);
	while (i < deltax)
	{
		increment = (i * edge_vector_magn) / deltax;
	
		zbuffer[screen_to_frame_coords(	app->main_window->width,
										app->main_window->height,
								(*start)[0] + increment * edge_vector[0] +
											app->main_window->width / 2,
								(*start)[1] + increment * edge_vector[1] +
											app->main_window->width / 2)] = 1;
	}
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
			t_mesh *mesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	int					i;
	// t_vec2				corners_on_screen[3];
	// t_vec3				normalized_normal;
	// uint32_t			*zbuffer;
	int					width = app->main_window->width;
	int					height = app->main_window->height;
	(void)width;
	(void)height;

	// zbuffer = app->main_window->zbuffer;
	(void)mesh;
	// screen_intersection(app, triangle, NULL);
	// ml_vector3_normalize(triangle->normal, normalized_normal);
	color = 0xffaaffff;
	//Here we borrow zbuffers memory to store the triangle's screen coordinates
	//the area will be filled right after with triangles z values (might be fragile af)
	// zbuffer[screen_to_frame_coords(width, height, + width / 2, + height / 2)] = 0;
	// paint_edge_to_buffer(uint32_t *zbuffer, t_vec2 *start, t_vec2 *end);
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
		if (triangle_intersection(triangle, &(camera->rays[i]), &intsec))
		{
			app->main_window->framebuffer[
				screen_to_frame_coords(app->main_window->width, app->main_window->height,
				(int)(camera->rays[i].dir[1]) + camera->screen_width / 2,
				(int)(camera->rays[i].dir[2]) + camera->screen_height / 2)] = color;
		}
		i++;
		//find a way to get uv data in fragment shader
		//mesh->shader->f(triangle, calculate_baryocoords(intsec), color);
		//find a way to get framebuffer data in and or out of this function
	}
	return (true);
}

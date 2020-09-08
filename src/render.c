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

// void		paint_edge_to_buffer(uint32_t *zbuffer, t_vec2 *start, t_vec2 *end)
// {
// 	t_vec2		dir;
// 	int			increment;
// 	int			deltax;

// 	deltax = endx - startx;
// 	while (i < deltax)
// 	{
// 		increment = i * magn(end - start) / deltax;
// 		xi = x0 + increment * dir.x;
// 		yi = y0 + increment * dir.y
// 		zbuffer.paint[x, y]
// 	}
// }

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
			t_mesh *mesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	int					i;
	t_vec2				corners_on_screen[3];
	t_vec3				normalized_normal;
	uint32_t			*zbuffer;
	int					width = app->main_window->zbuffer;
	int					height = app->main_window->zbuffer;

	zbuffer = app->main_window->zbuffer;
	(void)mesh;
	screen_intersection(app, triangle, NULL);
	ml_vector3_normalize(triangle->normal, normalized_normal);
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

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
	// project_triangle_onto_screen(app, triangle, screen_triangles, rays); //! WIP
}

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
			t_mesh *mesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	int					i;
	(void)mesh;
	t_vec3 normalized_normal;
	// screen_intersection(app, triangle, NULL);
	ml_vector3_normalize(triangle->normal, normalized_normal);
	color = 0xffaaffff;
	// color = (1 & 255) << 24 | (red & 255) << 16 | (green & 255) << 8 | (blue & 255);
	i = 0;
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
	// t_vec2	triangle_corners_screen[3];
	// screen_intersection(app, triangle, triangle_corners_screen);
	// (void)app;
	// (void)mesh;
	// (void)camera;
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

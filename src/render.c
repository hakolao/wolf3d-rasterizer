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

// t_mesh		*create_renderable_mesh(t_mesh *mesh, t_shader *shader,
// 									int *framebuffer)
// //framebuffer input here questionable?
// {
// 	t_mesh *mesh;

// 	return (mesh);
// }

t_bool		render_triangle(t_wolf3d *app, t_triangle *triangle,
			t_mesh *mesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	int					i;
	(void)mesh;
	t_vec3 normalized_normal;
	ml_vector3_normalize(triangle->normal, normalized_normal);
	int red = normalized_normal[0];
	int blue = normalized_normal[1];
	int green = normalized_normal[2];
	color = 0xffaaffff;
	(void)red;
	(void)blue;
	(void)green;
	// color = (1 & 255) << 24 | (red & 255) << 16 | (green & 255) << 8 | (blue & 255);
	if (triangle->vtc[2]->position[1] > -100.0)
		color = 0xff00ffff;
	i = 0;
	//project mesh bounding box on screen
	//decide later if bounding boxes for each triangle are needed for performance
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

// t_bool		render_mesh(t_mesh *mesh, t_camera *camera)
// {
// 	//for (vertex in triangle in mesh)
// 	//{
// 		//can't actually modify original vertex data
// 		//figure out if create temporary copy of all vertices
// 		//or some other way
// 	((*mesh).shader)->v(vertex, mesh, camera);
// 	//}
// 	while (triangles)
// 	{
// 		render_triangle(triangles[i], mesh, camera);
// 	}
// 	return (true);
// }

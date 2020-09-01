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

bool		render_triangle(t_triangle *triangle, t_mesh *mesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	int					i;
	(void)mesh;
	i =  -1;
	color = 0xffffffff;
	//project mesh bounding box on screen
	//decide later if bounding boxes for each triangle are needed for performance
	while (++i < camera->raycount)
	{
		if (triangle_intersection(triangle, camera->rays[i], &intsec))
		{
			camera->framebuffer[screen_to_frame_coords(camera->parent_scene,
													   camera->rays[i].dir[0],
													   camera->rays[i].dir[1])] = color;
			camera->framebuffer[4000] = color;
			camera->framebuffer[4001] = color;
			camera->framebuffer[4002] = color;
			camera->framebuffer[4001 + WIDTH] = color;
		}
		//find a way to get uv data in fragment shader
		//mesh->shader->f(triangle, calculate_baryocoords(intsec), color);
		//find a way to get framebuffer data in and or out of this function
		
	}
	return (true);
}

// bool		render_mesh(t_mesh *mesh, t_camera *camera)
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

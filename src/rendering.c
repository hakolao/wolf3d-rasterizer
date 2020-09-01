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
	int is;

	is = 0;
	t_intersection		intsec;
	// (void)triangle;
	int					color;
	int					i;
	(void)mesh;
	i =  -1;
	color = 0xffffffff;
	t_ray ray;
	i = 0;
	ray.dir[0] = camera->screen_dist;
	ray.dir[1] = -1 * (camera->screen_width + 1) / 2 + i % (int)camera->screen_width;
	ray.dir[2] = (camera->screen_height - 1) / 2 - floorf(i / camera->screen_width);
	ft_printf("x: %f\n", ray.dir[0]);
	ft_printf("y: %f\n", ray.dir[1]);
	ft_printf("z: %f\n", ray.dir[2]);
	ft_printf("======\n");
	i = -1;
	//project mesh bounding box on screen
	//decide later if bounding boxes for each triangle are needed for performance
	while (++i < camera->raycount)
	{
		ray.dir[0] = camera->screen_dist;
		ray.dir[1] = -1 * (camera->screen_width + 1) / 2 + i % (int)camera->screen_width;
		ray.dir[2] = (camera->screen_height - 1) / 2 - floorf(i / camera->screen_width);
		if (triangle_intersection(triangle, ray, &intsec))
		{
			is++;
			camera->framebuffer[screen_to_frame_coords(camera->parent_scene,
													   ray.dir[1] + WIDTH / 2,
													   ray.dir[2] + HEIGHT / 2)] = color;
		}
		//find a way to get uv data in fragment shader
		//mesh->shader->f(triangle, calculate_baryocoords(intsec), color);
		//find a way to get framebuffer data in and or out of this function
		
	}
	ft_printf("x: %f\n", ray.dir[0]);
	ft_printf("y: %f\n", ray.dir[1]);
	ft_printf("z: %f\n", ray.dir[2]);
	// ft_printf("intersections: %d\n", is);
	is = 0;
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

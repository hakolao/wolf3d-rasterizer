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

t_rmesh		*create_renderable_mesh(t_mesh *mesh, t_shader *shader,
									int *framebuffer)
//framebuffer input here questionable?
{
	t_rmesh *rmesh;

	return (rmesh);
}

bool		render_triangle(t_triangle *triangle, t_rmesh *rmesh, t_camera *camera)
{
	t_intersection		intsec;
	int					color;
	//project mesh bounding box on screen
	//decide later if bounding boxes for each triangle are needed for performance
	for (ray in mesh bounding box)
	{
	   triangle_intersection(triangle, ray, intsec);
	   //find a way to get uv data in fragment shader
	   rmesh->shader->f(triangle, calculate_baryocoords(intsec), color);
	   //find a way to get framebuffer data in and or out of this function
	   rmesh->framebuffer[x,y] = color;
	}
	return (true);
}

bool		render_mesh(t_rmesh *rmesh, t_camera *camera)
{
	for (vertex in triangle in rmesh)
	{
		//can't actually modify original vertex data
		//figure out if create temporary copy of all vertices
		//or some other way
		rmesh->shader->v(vertex, rmesh, camera);
	}
	while (triangles)
	{
		render_triangle(triangles[i], rmesh, camera);
	}
	return (true);
}

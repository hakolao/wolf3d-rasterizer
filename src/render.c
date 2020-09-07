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

void		init_screen_triangles(t_wolf3d *app, t_triangle *st)
{
	int			i;
	int			w;
	int			h;
	int			sd;

	w = app->main_window->width;
	h = app->main_window->height;
	sd = app->active_scene->main_camera->screen_dist;
	i = -1;
	while (++i < 3)
	{
		if (i == 0)
		{
			ml_vector3_copy((t_vec3){sd, w / 2, h / 2}, st[0].vtc[i]->position);
			ml_vector3_copy((t_vec3){sd, -w / 2, -h / 2},
									st[1].vtc[i]->position);
			continue;
		}
		ml_vector3_copy((t_vec3){sd, (3 - i * 2) * (w / 2), -(3 - i * 2) *
									h / 2}, st[0].vtc[i]->position);
		ml_vector3_copy((t_vec3){sd, (3 - i * 2) * (w / 2), -(3 - i * 2) *
									h / 2}, st[1].vtc[i]->position);
	}
}

void		screen_intersection(t_wolf3d *app, t_triangle *triangle,
								t_vec2 *corners_on_screen)
{
	t_ray		rays[3];
	t_triangle	screen_triangles[2];
	int			intersections[6];
	int			i;
	(void)corners_on_screen;
	if (((screen_triangles[0].vtc[i] =
				(t_vertex *)malloc(sizeof(t_vertex))) == NULL) ||
		((screen_triangles[1].vtc[i] =
				(t_vertex *)malloc(sizeof(t_vertex))) == NULL))
			ft_printf("Malloc Error in init_screen_triangles.\n"); //TODO properly handle errors

	init_screen_triangles(app, screen_triangles);
	ft_memset(intersections, 0 , sizeof(int) * 6);
	i = -1;
	while (++i < 3)
	{
		ft_memset(rays[i].origin, 0 , sizeof(rays[i].origin));
		ml_vector3_copy(triangle->vtc[i]->position, rays[i].dir);
		ml_vector3_normalize(rays[i].dir, rays[i].normalized_dir);
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
	screen_intersection(app, triangle, NULL);
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

// t_bool		render_mesh(t_wolf3d *app, t_mesh *mesh, t_camera *camera)
// {
// 	// int		i;
// 	// t_mesh	*mesh_copy;

// 	// copy_mesh(mesh, mesh_copy);
// 	// i = -1;
// 	// // for (vertex in triangle in mesh)
// 	// // {
// 	// // 	// can't actually modify original vertex data
// 	// // 	// figure out if create temporary copy of all vertices
// 	// // 	// or some other way
// 	// // ((*mesh).shader)->v(vertex, mesh, camera);
// 	// // }
// 	// while (i < mesh->triangle_count)
// 	// {
// 	// 	render_triangle(app, mesh->triangles[i], mesh, camera);
// 	// }
// 	return (true);
// }

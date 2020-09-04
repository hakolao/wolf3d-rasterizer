/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 16:45:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_ray	*precompute_rays(t_camera *camera)
{
	t_ray	*rays;
	int		x;
	int		y;
	int		size;
	t_vec3	dir;

	y = -1;
	size = camera->screen_width *camera->screen_height;
	if (!(rays = (t_ray*)malloc(sizeof(t_ray) * size)))
		return (NULL);
	camera->mallocsize = sizeof(t_ray) * size;
	camera->raycount = size;
	while (++y < camera->screen_height)
	{
		x = -1;
		while (++x < camera->screen_width)
		{
			dir[0] = camera->screen_dist;
			dir[1] = (float)(-x + (int)camera->screen_width / 2.0);
			dir[2] = (float)(-y + (int)camera->screen_height / 2.0);
			// ml_vector3_normalize(dir, dir);
			ft_printf("setting ray number: %d", y * (int)camera->screen_width + x);
			ft_printf(" / %d\n", camera->raycount);
			rays[y * (int)camera->screen_width + x] = new_ray(camera->origin, dir);
			ml_vector3_print(rays[y * camera->screen_width + x].dir);
		}
	}
	return (rays);
}

t_camera		*new_camera(t_scene *scene, float screen_distance)
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vec3_set_all(camera->origin, 0);
	camera->screen_width = WIDTH * VIEW_SCALE;
	camera->screen_height = HEIGHT * VIEW_SCALE;
	camera->screen_dist = screen_distance * VIEW_SCALE;
	camera->fovx = 2 * (atan(camera->screen_width / (2 * camera->screen_dist)));
	camera->fovy = 2 * (atan(camera->screen_height /
						(2 * camera->screen_dist)));
	ml_set_orientation_base(camera->orientation, VEC_FORWARD, VEC_LEFT, VEC_UP);
	camera->raycount = 0;
	camera->rays = precompute_rays(camera);
	camera->framebuffer = scene->main_window->framebuffer;
	camera->parent_scene = scene; 
	return (camera);
}

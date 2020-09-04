/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 16:03:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_ray	*precompute_rays(t_camera *camera)
{
	t_ray		*rays;
	int32_t		x;
	int32_t		y;
	int32_t		size;
	t_vec3		dir;

	y = -1;
	size = camera->screen_width *camera->screen_height;
	if (!(rays = (t_ray*)malloc(sizeof(t_ray) * size)))
		return (NULL);
	camera->mallocsize = sizeof(t_ray) * size;
	camera->raycount = size;
	while (++y < (int32_t)camera->screen_height)
	{
		x = -1;
		while (++x < (int32_t)camera->screen_width)
		{
			dir[0] = camera->screen_dist;
			dir[1] = -(float)x + (float)camera->screen_width / 2.0;
			dir[2] = -(float)y + (float)camera->screen_height / 2.0;
			rays[y * camera->screen_width + x] = new_ray(camera->origin, dir);
			ml_vector3_normalize(rays[y * camera->screen_width + x].dir,
							rays[y * camera->screen_width + x].normalized_dir);
		}
	}
	x = 0;//debug only delete after bug found
	while (x < camera->raycount)
	{
		if (rays[x].dir[1] < (float)(-1.0 * camera->screen_width / 2) ||
			rays[x].dir[2] < (float)(-1.0 * camera->screen_width / 2))
			
			{
				ft_printf("ray out of bounds lower at index: %d\n", x);
				ft_printf("ray y: %f\n", rays[x].dir[1]);
				ft_printf("ray z: %f\n", rays[x].dir[2]);
			}
		if ( rays[x].dir[2] > (float)(camera->screen_width / 2) ||
			rays[x].dir[2] > (float)(camera->screen_width / 2))
			{
				ft_printf("ray out of bounds higher at index: %d\n", x);
				ft_printf("ray y: %f\n", rays[x].dir[1]);
				ft_printf("ray z: %f\n", rays[x].dir[2]);
			}
			x++;
	}/////
	return (rays);
}

void			update_camera(t_wolf3d *app)
{
	t_camera	*camera;

	camera = app->active_scene->main_camera;
	camera->screen_width = app->main_window->width * VIEW_SCALE;
	camera->screen_height = app->main_window->height * VIEW_SCALE;
	camera->screen_dist = app->main_window->width * VIEW_SCALE;
	camera->fovx = 2 * (atan(camera->screen_width / (2 * camera->screen_dist)));
	camera->fovy = 2 * (atan(camera->screen_height /
						(2 * camera->screen_dist)));
	ml_set_orientation_base(camera->orientation, VEC_FORWARD, VEC_LEFT, VEC_UP);
	camera->raycount = 0;
	if (camera->rays == NULL)
		camera->rays = precompute_rays(camera);
	else
	{
		free(camera->rays);
		camera->rays = precompute_rays(camera);
	}
	camera->parent_scene = app->active_scene;
}

t_camera		*new_camera()
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vec3_set_all(camera->origin, 0);
	camera->rays = NULL;
	return (camera);
}

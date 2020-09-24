/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/24 16:46:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_ray	*rays_create(t_wolf3d *app, t_camera *camera)
{
	float	scale;
	float	aspect_ratio;
	int		x;
	int		y;
	int		i;
	t_ray	*rays;

	if (!(rays = (t_ray*)malloc(sizeof(t_ray) *
		app->main_window->width * app->main_window->height)))
		return (NULL);
	scale = tan(ml_rad((camera->fov * 0.5)));
 	aspect_ratio = app->main_window->width / (float)app->main_window->height;
	y = -1;
	i = 0;
	while (++y < app->main_window->height)
	{
		x = -1;
		while (++x < app->main_window->width)
		{
			rays[i].dir[0] = (2 * (x + 0.5) / (float)app->main_window->width - 1)
				* aspect_ratio * scale;
			rays[i].dir[1] = (1 - 2 * (y + 0.5) / (float)app->main_window->height) *
				scale;
			rays[i].dir[2] = -1.0;
			ml_vector3_copy(camera->origin, rays[i].origin);
			ml_vector3_sub(rays[i].dir, rays[i].origin, rays[i].dir);
			ml_vector3_normalize(rays[i].dir, rays[i].dir);
			i++;
		}
	}
	return (rays);
}

void			update_camera(t_wolf3d *app)
{
	t_camera	*camera;

	camera = app->active_scene->main_camera;
	camera->fov = 90;
	camera->screen_dist = app->main_window->width;
	camera->width = app->main_window->width;
	camera->height = app->main_window->height;
	if (camera->rays == NULL)
		camera->rays = rays_create(app, camera);
	else
	{
		free(camera->rays);
		camera->rays = rays_create(app, camera);
	}
}

t_camera		*new_camera()
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vector3_set_all(camera->origin, 0);
	camera->rays = NULL;
	return (camera);
}

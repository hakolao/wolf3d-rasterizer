/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 18:39:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			update_camera_view(t_player *player)
{
	t_vec3	center;

	ml_vector3_add(player->pos, player->forward, center);
	ml_matrix4_lookat(player->pos, center, player->up,
		player->camera.view);
}

static t_ray	*precompute_rays(t_camera *camera)
{
	t_ray	*rays;
	Uint32	i;
	Uint32	size;
	t_vec3	dir;

	i = 0;
	size = camera->screen_width * camera->screen_height;
	ft_printf("width: %d\n", camera->screen_width);
	ft_printf("height: %d\n", camera->screen_height);
	ft_printf("size: %d\n", size);
	if (!(rays = (t_ray *)malloc(sizeof(t_ray) * size)))
		return (NULL);
	while (i < size)
	{
		dir[0] = camera->screen_dist;
		dir[1] = -1 * (camera->screen_width + 1) / 2 + i % (int)camera->screen_width;
		dir[2] = (camera->screen_height - 1) / 2 - floorf(i / camera->screen_width);
		ml_vector3_normalize(dir, dir);
		rays[i] = new_ray(camera->origin, dir);
		i++;
	}
	camera->raycount = size;
	return (rays);
}

t_camera		*new_camera(t_scene *scene, float screen_distance)
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vec3_set_all(camera->origin, 0);
	camera->screen_width = (float)WIDTH * VIEW_SCALE;
	camera->screen_height = (float)HEIGHT * VIEW_SCALE;
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

void			init_camera(t_player *player)
{
	ml_matrix4_id(player->camera.model);
	update_camera_view(player);
	ml_matrix4_projection(ML_PERSPECTIVE,
		(t_canvas){.width = WIDTH, .height = HEIGHT, .near = 0.1, .far = 10.0,
		.fov = 45.0f}, player->camera.projection);
	player->camera.projection[1][1] *= -1;
}

void			camera_transform(t_camera *camera, t_vec4 vertex, t_vec4 res)
{
	t_mat4	tmp;
	t_mat4	transform;

	ml_matrix4_mul(camera->model, camera->projection, tmp);
	ml_matrix4_mul(tmp, camera->view, transform);
	ml_matrix4_mul_vec4(transform, vertex, res);
	// Don't divide if behind screen so screen check can be done in drawing
	if (res[3] > 0)
	{
		res[0] /= res[3];
		res[1] /= res[3];
		res[2] /= res[3];
		res[3] /= res[3];
	}
}

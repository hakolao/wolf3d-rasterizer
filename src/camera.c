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

void		update_camera_view(t_player *player)
{
	t_vec3	center;

	ml_vector3_add(player->pos, player->forward, center);
	ml_matrix4_lookat(player->pos, center, player->up,
		player->camera.view);
}

void		init_camera(t_player *player)
{
	ml_matrix4_id(player->camera.model);
	update_camera_view(player);
	ml_matrix4_projection(ML_PERSPECTIVE,
		(t_canvas){.width = WIDTH, .height = HEIGHT, .near = 0.1, .far = 10.0,
		.fov = 45.0f}, player->camera.projection);
	player->camera.projection[1][1] *= -1;
}

void		camera_transform(t_camera *camera, t_vec4 vertex, t_vec4 res)
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

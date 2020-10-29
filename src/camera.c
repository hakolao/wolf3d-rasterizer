/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 16:48:34 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Sets camera viewbox using planes. The order is:
**	Near, far, top, right, bottom, left
*/

static void set_camera_viewbox(t_wolf3d *app, t_plane *viewplanes)
{
	t_vec3	top;
	t_vec3	right;
	t_vec3	bottom;
	t_vec3	left;
	t_mat4	rotation;
	float	aspect;

	aspect = (float)app->active_scene->main_camera->width /
			(float)app->active_scene->main_camera->height;
	ml_vector3_set(top, 0, 1, 0);
	ml_vector3_set(right, -1, 0, 0);
	ml_vector3_set(bottom, 0, -1, 0);
	ml_vector3_set(left, 1, 0, 0);
	ml_matrix4_general_rotation((t_vec3){1, 0, 0},(M_PI / 12) * 1.1, rotation);
	ml_matrix4_mul_vec3(rotation, top, top);
	ml_matrix4_general_rotation((t_vec3){-1, 0, 0}, (M_PI / 12) * 1.1, rotation);
	ml_matrix4_mul_vec3(rotation, bottom, bottom);
	ml_matrix4_general_rotation((t_vec3){0, 1, 0}, (M_PI / 12) * aspect, rotation);
	ml_matrix4_mul_vec3(rotation, right, right);
	ml_matrix4_general_rotation((t_vec3){0, -1, 0}, (M_PI / 12) * aspect, rotation);
	ml_matrix4_mul_vec3(rotation, left, left);
	viewplanes[0].d = NEAR_CLIP_DIST;
	ml_vector3_set(viewplanes[0].origin, 0, 0, -NEAR_CLIP_DIST);//!change back to * 1
	ml_vector3_set(viewplanes[0].normal, 0, 0, -1);
	viewplanes[1].d = FAR_CLIP_DIST;
	ml_vector3_set(viewplanes[1].normal, 0, 0, 1);
	viewplanes[2].d = 0.0;
	ml_vector3_copy(top, viewplanes[2].normal);
	viewplanes[3].d = 0.0;
	ml_vector3_copy(right, viewplanes[3].normal);
	viewplanes[4].d = 0.0;
	ml_vector3_copy(bottom, viewplanes[4].normal);
	viewplanes[5].d = 0.0;
	ml_vector3_copy(left, viewplanes[5].normal);
}

void			update_camera(t_wolf3d *app)
{
	t_camera	*camera;

	camera = app->active_scene->main_camera;
	camera->fov = 90;
	camera->screen_dist = app->window->width;
	camera->width = app->window->width;
	camera->height = app->window->height;
	camera->near_clip = -NEAR_CLIP_DIST;
	camera->far_clip = -FAR_CLIP_DIST;
	ml_vector3_set(camera->screen.origin, 0, 0, -app->window->width);
	ml_vector3_set(camera->screen.normal, 0, 0, 1);
	set_camera_viewbox(app, camera->viewplanes);
}

t_camera		*new_camera()
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vector3_set_all(camera->origin, 0);
	return (camera);
}

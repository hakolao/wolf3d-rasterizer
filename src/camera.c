/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 17:06:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			update_camera(t_wolf3d *app)
{
	t_camera	*camera;

	camera = app->active_scene->main_camera;
	camera->fov = 90;
	camera->screen_dist = app->main_window->width;
	camera->width = app->main_window->width;
	camera->height = app->main_window->height;
	camera->near_clip = -NEAR_CLIP_DIST;
	camera->far_clip = FAR_CLIP_DIST;
}

/*
**	Sets camera viewbox using planes. The order is:
**	Near, far, top, right, bottom, left
*/

static void		init_camera_viewbox(t_wolf3d *app, t_plane *viewplanes)
{
	ml_vector3_set(viewplanes[0].origin, 0, 0,
					app->active_scene->main_camera->near_clip);
	ml_vector3_set(viewplanes[0].normal, 0, 0, -1);

	ml_vector3_set(viewplanes[1].origin, 0, 0,
					app->active_scene->main_camera->far_clip);
	ml_vector3_set(viewplanes[1].normal, 0, 0, 1);

	ml_vector3_set(viewplanes[2].origin, 0, 0,
					app->active_scene->main_camera->near_clip);
	ml_vector3_set(viewplanes[2].normal, 0, 0, -1);

	ml_vector3_set(viewplanes[3].origin, 0, 0,
					app->active_scene->main_camera->near_clip);
	ml_vector3_set(viewplanes[3].normal, 0, 0, -1);

	ml_vector3_set(viewplanes[4].origin, 0, 0,
					app->active_scene->main_camera->near_clip);
	ml_vector3_set(viewplanes[4].normal, 0, 0, -1);

	ml_vector3_set(viewplanes[5].origin, 0, 0,
					app->active_scene->main_camera->near_clip);
	ml_vector3_set(viewplanes[5].normal, 0, 0, -1);

}

t_camera		*new_camera()
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vector3_set_all(camera->origin, 0);
	init_camera_viewbox(camera->viewplanes);
	return (camera);
}

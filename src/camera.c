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
	viewplanes[0].d = NEAR_CLIP_DIST;
	ml_vector3_set(viewplanes[0].normal, 0, 0, -1);

	viewplanes[1].d = FAR_CLIP_DIST;
	ml_vector3_set(viewplanes[1].normal, 0, 0, 1);

	viewplanes[2].d = app->window->height / 2;
	ml_vector3_set(viewplanes[2].normal, 0, 1, 0);

	viewplanes[3].d =  app->window->width / 2;
	ml_vector3_set(viewplanes[3].normal, -1, 0, 0);

	viewplanes[4].d = app->window->height / 2;
	ml_vector3_set(viewplanes[4].normal, 0, -1, 0);
	
	viewplanes[5].d = app->window->width / 2;
	ml_vector3_set(viewplanes[5].normal, 1, 0, 0);
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

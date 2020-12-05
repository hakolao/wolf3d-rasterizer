/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/05 18:11:35 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Sets camera viewbox using planes. The order is:
**	Near, far, top, right, bottom, left
*/

static void set_camera_viewbox(t_plane *viewplanes)
{
	viewplanes[0].d = NEAR_CLIP_DIST;
	ml_vector3_set(viewplanes[0].origin, 0, 0, -NEAR_CLIP_DIST);
	ml_vector3_set(viewplanes[0].normal, 0, 0, -1);
}

void			update_camera(t_wolf3d *app)
{
	t_camera	*camera;

	camera = app->active_scene->main_camera;
	camera->near_clip = -NEAR_CLIP_DIST;
	camera->far_clip = -FAR_CLIP_DIST;
	ml_vector3_set(camera->screen.origin, 0, 0, -app->window->width * 0.7);
	ml_vector3_set(camera->screen.normal, 0, 0, 1);
	set_camera_viewbox(camera->viewplanes);
}

t_camera		*new_camera()
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vector3_set_all(camera->origin, 0);
	return (camera);
}

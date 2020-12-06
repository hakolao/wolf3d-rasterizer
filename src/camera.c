/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 13:32:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 02:14:23 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Sets camera viewbox using planes. The order is:
**	Near, top, right, bottom, left
*/

static void set_camera_viewbox(t_wolf3d *app, t_camera *camera)
{
	// float	half_height;
	// float	half_width;
	// t_vec3	dirs[4];
	// t_vec3	screen_origin;
	// t_vec3	add;

	camera->viewplanes[0].d = NEAR_CLIP_DIST;
	ml_vector3_set(camera->viewplanes[0].origin, 0, 0, -NEAR_CLIP_DIST);
	ml_vector3_set(camera->viewplanes[0].normal, 0, 0, -1);
	(void)app;
	// http://davidlively.com/programming/graphics/frustum-calculation-and-culling-hopefully-demystified/
	// half_height = app->window->height / 2.0;
	// half_width = app->window->width / 2.0;
	// ml_vector3_mul(app->player.forward, ml_vector3_mag(camera->screen.origin),
	// 	add);
	// ml_vector3_add(app->player.pos, add, screen_origin);
	// ml_vector3_mul(app->player.up, half_height, dirs[0]);
	// ml_vector3_mul(app->player.sideways, half_width, dirs[1]);
	// ml_vector3_mul(app->player.up, -half_width, dirs[2]);
	// ml_vector3_mul(app->player.sideways, -half_width, dirs[3]);
	// ml_vector3_add(screen_origin, dirs[0], camera->viewplanes[1].origin);
	// ml_vector3_cross(screen_origin, camera->viewplanes[1].origin,
	// 	camera->viewplanes[1].normal);
	// ml_vector3_add(screen_origin, dirs[1], camera->viewplanes[2].origin);
	// ml_vector3_cross(screen_origin, camera->viewplanes[2].origin,
	// 	camera->viewplanes[2].normal);
	// ml_vector3_add(screen_origin, dirs[2], camera->viewplanes[3].origin);
	// ml_vector3_cross(screen_origin, camera->viewplanes[3].origin,
	// 	camera->viewplanes[3].normal);
	// ml_vector3_add(screen_origin, dirs[3], camera->viewplanes[4].origin);
	// ml_vector3_cross(screen_origin, camera->viewplanes[4].origin,
	// 	camera->viewplanes[4].normal);
}

void			update_camera(t_wolf3d *app)
{
	t_camera	*camera;

	camera = app->active_scene->main_camera;
	camera->near_clip = -NEAR_CLIP_DIST;
	camera->far_clip = -FAR_CLIP_DIST;
	ml_vector3_set(camera->screen.origin, 0, 0, -app->window->width * 0.7);
	ml_vector3_set(camera->screen.normal, 0, 0, 1);
	set_camera_viewbox(app, camera);
}

t_camera		*new_camera()
{
	t_camera	*camera;

	if (!(camera = (t_camera*)malloc(sizeof(t_camera))))
		return (NULL);
	ml_vector3_set_all(camera->origin, 0);
	return (camera);
}

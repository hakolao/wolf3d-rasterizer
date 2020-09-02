/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 13:49:05 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_scene		*new_scene(t_wolf3d *app, t_scenedata *data)
{
	t_scene		*scene;

	error_check(!(scene = (t_scene*)malloc(sizeof(t_scene))),
		"Failed to malloc scene");
	scene->main_window = app->main_window;
	app->active_scene = scene;
	scene->main_camera = new_camera(app->active_scene, app->main_window->width);
	(void)data;
	(void)app;
	return (scene);
}

void		destroy_scene(t_wolf3d *app)
{
	(void)app;
	return ;
}
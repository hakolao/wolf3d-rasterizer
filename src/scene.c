/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 19:30:52 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		select_scene(t_wolf3d *app, t_scene_id scene_id)
{
	t_scene_data		data;

	data.scene_id = scene_id;
	if (scene_id == scene_id_main_menu)
	{
		data.level = 0;
		data.menu_options[0] = "Start Game";
		data.menu_options[1] = "Load Game";
		data.menu_options[2] = "Quit";
		data.menu_option_count = 3;
		data.main_camera = NULL;
	}
	else if (scene_id == scene_id_main_game)
	{
		data.level = 1;
		data.menu_option_count = 0;
		data.main_camera = new_camera();
	}
	app->active_scene = new_scene(app, &data);
}

void			set_active_scene(t_wolf3d *app, t_scene_id to_scene)
{
	if (app->active_scene != NULL)
		destroy_scene(app->active_scene);
	select_scene(app, to_scene);
}

t_scene		*new_scene(t_wolf3d *app, t_scene_data *data)
{
	t_scene		*scene;

	error_check(!(scene = (t_scene*)malloc(sizeof(t_scene))),
		"Failed to malloc scene");
	scene->main_window = app->main_window;
	app->active_scene = scene;
	scene->scene_id = data->scene_id;
	ft_memcpy(scene->menu_options, data->menu_options,
		sizeof(char*)*data->menu_option_count);
	scene->menu_option_count = data->menu_option_count;
	scene->selected_option = 0;
	scene->main_camera = data->main_camera;
	if (scene->main_camera)
		update_camera(app);
	return (scene);
}

void		destroy_scene(t_scene *scene)
{
	// ToDo: Destroy objects
	if (scene->main_camera)
	{
		free(scene->main_camera->rays);
		free(scene->main_camera);
	}
	scene = NULL;
	return ;
}

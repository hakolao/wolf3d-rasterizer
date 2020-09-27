/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 17:46:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		init_scene_transform(t_wolf3d *app, t_scene_data *data)
{
	ml_matrix4_id(data->world_rotation);
	ml_matrix4_id(data->world_scale);
	ml_matrix4_id(data->world_transform);
	data->world_scale[0][0] = app->main_window->width / 4.0 - 1.0;
	data->world_scale[1][1] = app->main_window->width / 4.0 - 1.0;
	data->world_scale[2][2] = app->main_window->width / 4.0 - 1.0;
	ml_matrix4_translation(0, 0, app->main_window->width, data->world_translation);
}

static void		select_scene(t_wolf3d *app, t_scene_id scene_id)
{
	t_scene_data		data;

	data.scene_id = scene_id;
	if (scene_id == scene_id_main_menu)
	{
		data.menu_options[0] = "Start Game";
		data.menu_options[1] = "Load Game";
		data.menu_options[2] = "Quit";
		data.menu_option_count = 3;
		data.main_camera = NULL;
		data.objects = NULL;
		data.object_count = 0;
	}
	else if (scene_id == scene_id_main_game)
	{
		data.level = 0;
		data.menu_option_count = 0;
		data.main_camera = new_camera();
		data.objects = create_scene1_objects(&data.object_count);
		init_scene_transform(app, &data);
	}
	app->active_scene = new_scene(app, &data);
}

void			set_active_scene(t_wolf3d *app, t_scene_id to_scene)
{
	if (app->active_scene != NULL)
		destroy_scene(app->active_scene);
	select_scene(app, to_scene);
	// debug_scene(app->active_scene);
}

t_scene			*new_scene(t_wolf3d *app, t_scene_data *data)
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
	scene->object_count = data->object_count;
	scene->objects = data->objects;
	ft_memcpy(scene->world_transform, data->world_transform, sizeof(t_mat4));
	ft_memcpy(scene->world_rotation, data->world_rotation, sizeof(t_mat4));
	ft_memcpy(scene->world_translation, data->world_translation, sizeof(t_mat4));
	ft_memcpy(scene->world_scale, data->world_scale, sizeof(t_mat4));
	update_world_scale(scene, scene->world_scale);
	update_world_rotation(scene, scene->world_rotation);
	update_world_translation(scene, scene->world_translation);
	if (scene->main_camera)
		update_camera(app);
	return (scene);
}

void			destroy_scene(t_scene *scene)
{
	int		i;

	if (scene->objects != NULL)
	{
		i = -1;
		while (++i < scene->object_count)
			destroy_object(scene->objects[i]);
		free(scene->objects);
		scene->objects = NULL;
	}
	if (scene->main_camera)
	{
		free(scene->main_camera);
		scene->main_camera = NULL;
	}
	free(scene);
	scene = NULL;
	return ;
}

void			debug_scene(t_scene *scene)
{
	int		i;
	int		j;
	int		k;

	ft_printf("Scene: %d\n"
	"objects: %d\n",
		scene->scene_id,
		scene->object_count);
	i = -1;
	while (++i < scene->object_count)
	{
		j = -1;
		while (++j < (int)scene->objects[i]->num_triangles)
		{
			ft_printf("Triangle: %d\n", j);
			k = -1;
			while (++k < 3)
				ml_vector3_print(
					scene->objects[i]->triangles[j].vtc[k]->position);
			ft_printf("Normal:\n");
			ml_vector3_print(scene->objects[i]->triangles[j].normal);
		}
	}
}

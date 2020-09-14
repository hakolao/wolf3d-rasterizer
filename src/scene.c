/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/14 18:35:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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
	}
	app->active_scene = new_scene(app, &data);
}

void			set_active_scene(t_wolf3d *app, t_scene_id to_scene)
{
	if (app->active_scene != NULL)
		destroy_scene(app->active_scene);
	select_scene(app, to_scene);
	debug_scene(app->active_scene);
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
		free(scene->main_camera->rays);
		free(scene->main_camera);
		scene->main_camera = NULL;
	}
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
		while (++j < (int)scene->objects[i]->mesh_triangle_count)
		{
			ft_printf("Triangle: %d\n", j);
			k = -1;
			while (++k < 3)
			{
				ml_vector3_print(
					scene->objects[i]->mesh_triangles[j].vtc[0]->position);	
				ml_vector3_print(
					scene->objects[i]->mesh_triangles[j].vtc[1]->position);	
				ml_vector3_print(
					scene->objects[i]->mesh_triangles[j].vtc[2]->position);	
			}
			ft_printf("Normal:\n");
			ml_vector3_print(scene->objects[i]->mesh_triangles[j].normal);
		}
	}
}
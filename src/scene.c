/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/15 15:41:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** Add objs to scene. ToDo: Also should pair them with textures
** and position them where they belong.
** Set scene & object specific transformations in scene data.
*/

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
		data.num_objects = 0;
	}
	else if (scene_id == scene_id_main_game)
	{
		data.level = 0;
		data.menu_option_count = 0;
		data.main_camera = new_camera();
		data.objects[0] = l3d_read_obj("assets/icosphere.obj");
		data.num_objects = 1;
		l3d_3d_object_scale(data.objects[0],
			app->window->width / 5.0,
			app->window->width / 5.0,
			app->window->width / 5.0);
		l3d_3d_object_translate(data.objects[0],
			0, 0, -app->window->width - 500);
	}
	app->active_scene = new_scene(&data);
	if (app->active_scene->main_camera)
		update_camera(app);
}

/*
** Initial transformations are set to id, and they change when player moves.
*/

t_scene			*new_scene(t_scene_data *data)
{
	t_scene		*scene;

	error_check(!(scene = (t_scene*)malloc(sizeof(t_scene))),
		"Failed to malloc scene");
	scene->scene_id = data->scene_id;
	ft_memcpy(scene->menu_options, data->menu_options,
		sizeof(char*)*data->menu_option_count);
	scene->menu_option_count = data->menu_option_count;
	scene->selected_option = 0;
	scene->main_camera = data->main_camera;
	scene->num_objects = data->num_objects;
	if (data->num_objects > 0)
	{
		ft_memmove(scene->objects, data->objects,
			sizeof(t_3d_object) * data->num_objects);
	}
	return (scene);
}

void			set_active_scene(t_wolf3d *app, t_scene_id to_scene)
{
	if (app->active_scene != NULL)
		destroy_scene(app->active_scene);
	select_scene(app, to_scene);
	// debug_scene(app->active_scene);
}

void			destroy_scene(t_scene *scene)
{
	int		i;

	i = -1;
	while (++i < (int)scene->num_objects)
		l3d_3d_object_destroy(scene->objects[i]);
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
		scene->num_objects);
	i = -1;
	while (++i < (int)scene->num_objects)
	{
		j = -1;
		while (++j < (int)scene->objects[i]->num_triangles)
		{
			ft_printf("Triangle: %d\n", j);
			k = -1;
			while (++k < 3)
				ml_vector3_print(
					scene->objects[i]->triangles[j].vtc[k]->pos);
			ft_printf("Normal:\n");
			ml_vector3_print(scene->objects[i]->triangles[j].normal);
		}
	}
}

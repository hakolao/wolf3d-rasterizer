/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/05 19:27:02 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		select_scene(void *app_ptr)
{
	t_scene_data		data;
	t_wolf3d			*app;

	app = app_ptr;
	app->is_first_render = true;
	if (app->active_scene != NULL)
		scene_destroy(app->active_scene);
	ft_memset(&data, 0, sizeof(data));
	data.scene_id = app->next_scene_id;
	if (data.scene_id == scene_id_main_menu)
		scene_main_menu_data_set(&data);
	else if (data.scene_id == scene_id_main_menu_settings)
		scene_settings_menu_data_set(&data);
	else if (data.scene_id == scene_id_main_game)
		scene_main_game_data_set(&data);
	app->active_scene = scene_new(&data);
	if (app->active_scene->map)
	{
		scene_objects_generate(app, app->active_scene);
		scene_minimap_init(app);
	}
	if (app->active_scene->main_camera)
		update_camera(app);
	app->is_loading = false;
}

/*
** Initial transformations are set to id, and they change when player moves.
*/

t_scene			*scene_new(t_scene_data *data)
{
	t_scene		*scene;

	error_check(!(scene = (t_scene*)malloc(sizeof(t_scene))),
		"Failed to malloc scene");
	ft_memset(scene, 0, sizeof(*scene));
	scene->scene_id = data->scene_id;
	ft_memcpy(scene->menu_options, data->menu_options,
		sizeof(char*)*data->menu_option_count);
	scene->menu_option_count = data->menu_option_count;
	scene->selected_option = 0;
	scene->main_camera = data->main_camera;
	scene->map_filename = data->map_filename;
	scene->triangle_ref = NULL;
	ft_memset(scene->skybox, 0, sizeof(t_3d_object*) * 6);
	if (data->num_assets_to_load > 0)
		scene_assets_load(scene, data);
	if (data->map_filename)
		scene_map_init(scene);
	return (scene);
}

/*
** Concurrently select's and loads next scene. Once done, select scene
** will turn app->is_loading to false;
*/

void			scene_next_select(t_wolf3d *app)
{
	app->is_loading = true;
	thread_pool_add_work(app->thread_pool,
		select_scene, app);
}

void			scene_destroy(t_scene *scene)
{
	if (scene->map_filename != NULL)
		ft_strdel(&scene->map_filename);
	if (scene->triangle_tree)
		l3d_kd_tree_destroy(scene->triangle_tree);
	if (scene->map != NULL)
		map_destroy(scene->map);
	if (scene->textures)
		scene_textures_destroy(scene);
	if (scene->normal_maps)
		scene_normal_maps_destroy(scene);
	if (scene->models)
		scene_models_destroy(scene);
	if (scene->skybox[0])
		scene_skybox_destroy(scene);
	if (scene->triangle_ref)
	{
		free(scene->triangle_ref);
		scene->triangle_ref = NULL;
	}
	scene_objects_destroy(scene);
	scene_camera_destroy(scene);
	free(scene);
	scene = NULL;
	return ;
}

void			scene_debug(t_scene *scene)
{
	int		i;

	ft_printf("Scene: %d\n"
	"objects: %d\n",
		scene->scene_id,
		scene->num_objects);
	i = -1;
	while (++i < (int)scene->num_objects)
	{
		l3d_3d_object_debug_print(scene->objects[i]);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/25 13:26:02 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		load_scene_assets(t_scene *scene, t_scene_data *data)
{
	int32_t		i;
	uint32_t	key;

	scene->models = hash_map_create(data->num_models);
	scene->textures = hash_map_create(data->num_models);
	i = -1;
	while (++i < (int32_t)data->num_models)
	{
		key = data->asset_keys[i];
		if (data->texture_files[i] != NULL && data->model_files[i] != NULL)
		{
			hash_map_add(scene->textures, key,
				l3d_read_bmp_image_32bit_rgba_surface(data->texture_files[i]));
			hash_map_add(scene->models, key,
				l3d_read_obj(data->model_files[i],
					hash_map_get(scene->textures, key)));
		}
	}
	scene->skybox_textures[0] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/front.bmp");
	scene->skybox_textures[1] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/left.bmp");
	scene->skybox_textures[2] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/top.bmp");
	scene->skybox_textures[3] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/back.bmp");
	scene->skybox_textures[4] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/right.bmp");
	scene->skybox_textures[5] = l3d_read_bmp_image_32bit_rgba_surface(
		"assets/skybox/bottom.bmp");
}

/*
** Fill assets here (3d model & texture pairs) and their corresponding
** feature keys.
** set_main_scene_assets sets these to the scene into a hashmap that the
** procedural generator can then access & copy from.
*/

static void		set_main_scene_data_assets(t_scene_data *data)
{
	data->texture_files[0] = "assets/textures/test_texture.bmp";
	data->texture_files[1] = "assets/textures/test_texture.bmp";
	data->texture_files[2] = "assets/textures/test_texture.bmp";
	data->texture_files[3] = "assets/textures/test_texture.bmp";
	data->texture_files[4] = "assets/textures/test_texture.bmp";
	data->texture_files[5] = "assets/textures/test_texture.bmp";
	data->texture_files[6] = NULL;
	data->texture_files[7] = NULL;
	data->texture_files[8] = NULL;
	data->texture_files[9] = NULL;
	data->model_files[0] = "assets/models/room_tiles/room_floor.obj";
	data->model_files[1] = NULL;
	data->model_files[2] = "assets/models/room_tiles/wall_up.obj";
	data->model_files[3] = "assets/models/room_tiles/wall_right.obj";
	data->model_files[4] = "assets/models/room_tiles/wall_down.obj";
	data->model_files[5] = "assets/models/room_tiles/wall_left.obj";
	data->model_files[6] = NULL;
	data->model_files[7] = NULL;
	data->model_files[8] = NULL;
	data->model_files[9] = NULL;
	data->asset_keys[0] = c_floor;
	data->asset_keys[1] = c_floor_start;
	data->asset_keys[2] = c_wall_up;
	data->asset_keys[3] = c_wall_right;
	data->asset_keys[4] = c_wall_down;
	data->asset_keys[5] = c_wall_left;
	data->asset_keys[6] = c_block_nw;
	data->asset_keys[7] = c_block_ne;
	data->asset_keys[8] = c_block_se;
	data->asset_keys[9] = c_block_sw;
	data->num_models = 10;
}

static void		select_scene(void *app_ptr)
{
	t_scene_data		data;
	t_wolf3d			*app;

	app = app_ptr;
	if (app->active_scene != NULL)
		destroy_scene(app->active_scene);
	ft_memset(&data, 0, sizeof(data));
	data.scene_id = app->next_scene_id;
	if (data.scene_id == scene_id_main_menu)
	{
		data.menu_options[0] = "Start Game";
		data.menu_options[1] = "Load Game";
		data.menu_options[2] = "Settings";
		data.menu_options[3] = "Quit";
		data.menu_option_count = 4;
		ft_printf("Select scene main menu\n");
	}
	if (data.scene_id == scene_id_main_menu_settings)
	{
		data.menu_options[0] = "1280x720";
		data.menu_options[1] = "1920x1080";
		data.menu_options[2] = "Back";
		data.menu_option_count = 3;
	}
	else if (data.scene_id == scene_id_main_game)
	{
		data.menu_options[0] = "Save";
		data.menu_options[1] = "Main Menu";
		data.menu_options[2] = "Exit Game";
		data.menu_option_count = 3;
		data.main_camera = new_camera();
		data.map_filename = ft_strdup("maps/bigone");
		set_main_scene_data_assets(&data);
	}
	app->active_scene = new_scene(&data);
	if (app->active_scene->map)
		generate_scene_objects(app, app->active_scene);
	if (app->active_scene->main_camera)
		update_camera(app);
	app->is_loading = false;
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
	scene->map_filename = data->map_filename;
	scene->map = NULL;
	scene->models = NULL;
	scene->textures = NULL;
	ft_memset(scene->skybox, 0, sizeof(t_3d_object*) * 6);
	if (data->num_models > 0)
		load_scene_assets(scene, data);
	if (data->map_filename)
		read_and_init_scene_map(scene);
	return (scene);
}

/*
** Concurrently select's and loads next scene. Once done, select scene
** will turn app->is_loading to false;
*/

void			select_next_scene(t_wolf3d *app)
{
	app->is_loading = true;
	thread_pool_add_work(app->thread_pool,
		select_scene, app);
}

void			destroy_scene(t_scene *scene)
{
	int			i;
	t_surface	*texture;
	uint32_t	key;
	t_3d_object	*model;

	ft_printf("Destroy scene\n");
	if (scene->map_filename != NULL)
		ft_strdel(&scene->map_filename);
	if (scene->bullet_tree)
		l3d_kd_tree_destroy(scene->bullet_tree);
	if (scene->map != NULL)
	{
		free(scene->map->grid);
		free(scene->map);
	}
	if (scene->textures)
	{
		i = -1;
		while (++i < (int32_t)sizeof(uint32_t) * 4)
		{
			key = 1 << i;
			if ((texture = hash_map_get(scene->textures, key)))
				free(texture->pixels);
		}
		hash_map_destroy_free(scene->textures);
	}
	if (scene->models)
	{
		i = -1;
		while (++i < (int32_t)sizeof(uint32_t) * 4)
		{
			key = 1 << i;
			if ((model = hash_map_get(scene->models, key)))
				l3d_3d_object_destroy(model);
		}
		hash_map_destroy(scene->models);
	}
	if (scene->skybox[0])
	{
		i = -1;
		while (++i < 6)
		{
			l3d_3d_object_destroy(scene->skybox[i]);
			free(scene->skybox_textures[i]->pixels);
			free(scene->skybox_textures[i]);
		}
	}
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

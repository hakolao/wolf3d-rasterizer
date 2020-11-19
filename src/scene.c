/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/19 19:55:52 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		set_main_scene_textures(t_scene_data *data)
{
	int32_t		i;
	uint32_t	key;
	char		*filename;

	data->texture_files = hash_map_create(64);
	data->textures = hash_map_create(64);
	hash_map_add(data->texture_files, c_floor,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_wall_up,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_wall_right,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_wall_down,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_wall_left,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_block_ne,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_block_nw,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_block_se,
		"assets/textures/test_texture_small.bmp");
	hash_map_add(data->texture_files, c_block_sw,
		"assets/textures/test_texture_small.bmp");
	i = -1;
	while (i < (int32_t)sizeof(uint32_t) * 4)
	{
		key = 1 << i;
		if ((filename = hash_map_get(data->texture_files, key)))
			hash_map_add(data->textures, key,
				l3d_read_bmp_image_32bit_rgba_surface(filename));
	}
	hash_map_destroy(data->model_files);
}

static void		set_main_scene_models(t_scene_data *data)
{
	int32_t		i;
	uint32_t	key;
	char		*filename;

	data->model_files = hash_map_create(64);
	data->models = hash_map_create(64);
	hash_map_add(data->model_files, c_floor,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_wall_up,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_wall_right,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_wall_down,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_wall_left,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_block_ne,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_block_nw,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_block_se,
		"assets/models/room_tiles/room_floor.obj");
	hash_map_add(data->model_files, c_block_sw,
		"assets/models/room_tiles/room_floor.obj");
	i = -1;
	while (i < (int32_t)sizeof(uint32_t) * 4)
	{
		key = 1 << i;
		if ((filename = hash_map_get(data->model_files, key)))
			hash_map_add(data->models, key,
				l3d_read_obj(filename, hash_map_get(data->textures, key)));
	}
	hash_map_destroy_free(data->model_files);
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
		data.map_filename = NULL;
	}
	else if (scene_id == scene_id_main_game)
	{
		data.level = 0;
		data.menu_option_count = 0;
		data.main_camera = new_camera();
		data.map_filename = ft_strdup("maps/okko");
		set_main_scene_textures(&data);
		set_main_scene_models(&data);
	}
	app->active_scene = new_scene(&data);
	if (data.map_filename)
		read_map_to_scene(app, app->active_scene, data.map_filename);
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
	scene->map_filename = data->map_filename;
	scene->map = NULL;
	scene->textures = data->textures;
	scene->models = data->models;
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
	int			i;
	t_surface	*texture;
	uint32_t	key;

	if (scene->map_filename != NULL)
		ft_strdel(&scene->map_filename);
	if (scene->map != NULL)
	{
		free(scene->map->grid);
		free(scene->map);
	}
	i = -1;
	while (i < (int32_t)sizeof(uint32_t) * 4)
	{
		key = 1 << i;
		if ((texture = hash_map_get(scene->textures, key)))
			free(texture->pixels);
	}
	hash_map_destroy_free(scene->textures);
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

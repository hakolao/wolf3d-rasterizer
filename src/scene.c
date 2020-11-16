/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:57:26 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		scene_set_triangle_refs(t_scene *scene)
{
	int		i;
	int		j;
	int		k;
	int		num_triangles;

	i = -1;
	num_triangles = 0;
	k = 0;
	while (++i < (int)scene->num_objects)
	{
		j = -1;
		while (++j < scene->objects[i]->num_triangles)
		{
			scene->triangle_ref[k] = &scene->objects[i]->triangles[j];
			num_triangles++;
			k++;
		}
	}
	scene->num_triangles = num_triangles;
}

/*
** // ToDo: Clean up bois!
*/

static void		read_map_to_scene(t_wolf3d *app,
						t_scene *scene, const char *map_filename)
{
	t_file_contents	*file;
	int32_t			x;
	int32_t			y;
	float			unit_size;
	uint32_t		cell;
	int32_t			obj_i;

	error_check(!(scene->map = malloc(sizeof(t_wolf3d_map))),
		"Failed to malloc map");
	error_check(!(scene->map->grid =
		malloc(sizeof(uint32_t) * MAP_SIZE * MAP_SIZE)),
		"Failed to malloc map grid");
	if (!(file = read_file(map_filename)))
		exit(EXIT_FAILURE);
	ft_memcpy(scene->map->grid, file->buf, file->size);
	destroy_file_contents(file);
	scene->map->size = MAP_SIZE;
	unit_size = app->window->width;
	y = -1;
	obj_i = 0;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
		{
			cell = scene->map->grid[y * MAP_SIZE + x];
			if ((cell & c_floor_start))
			{
				init_player(app,
					(t_vec3){(float)y * unit_size - (float)unit_size / 2.0, 0,
					(float)x * unit_size - (float)unit_size / 2.0});
			}
			if ((cell & c_floor))
			{
				scene->objects[obj_i] =
					l3d_read_obj("assets/models/room_tiles/room_floor.obj",
								"assets/textures/test_texture_small.bmp");
				l3d_3d_object_scale(scene->objects[obj_i],
									app->window->width / 1.0,
									app->window->width / 1.0,
									app->window->width / 1.0);
				l3d_3d_object_translate(scene->objects[obj_i],
					y * unit_size - unit_size / 2.0, PLAYER_HEIGHT * 1,
					-x * unit_size - unit_size / 2.0);
				obj_i++;
			}
			// ToDo Read other parts too, walls etc. Clean up.
		}
	}
	scene->bullet_tree = NULL;
	scene->num_objects = obj_i;
	if (scene->num_objects > 0)
	{
		scene_set_triangle_refs(scene);
		l3d_kd_tree_create_or_update(&scene->bullet_tree,
			scene->triangle_ref, scene->num_triangles);
	}
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

	if (scene->map_filename != NULL)
		ft_strdel(&scene->map_filename);
	if (scene->map != NULL)
	{
		free(scene->map->grid);
		free(scene->map);
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

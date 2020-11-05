/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/30 16:57:47 by ohakola          ###   ########.fr       */
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
		// data.objects[0] = l3d_read_obj("assets/icosphere_massive.obj",
		// 				"assets/IcosphereUV_large.bmp");
		// data.objects[0] = l3d_read_obj("assets/models/corners/corner_ceiling.obj",
		// 							   "assets/textures/test_texture_small.bmp");
		// data.objects[1] = l3d_read_obj("assets/models/corners/corner_walls.obj",
		// 							   "assets/textures/test_texture_small.bmp");
		// data.objects[2] = l3d_read_obj("assets/models/corners/corner_floor.obj",
		// 							   "assets/textures/test_texture_small.bmp");
		data.objects[0] = l3d_read_obj("assets/models/turn_right/turn_right_test.obj",
									   "assets/textures/test_texture.bmp");
		// data.objects[1] = l3d_read_obj("assets/models/turn_right/turn_right_wall.obj",
		// 							   "assets/textures/test_texture_small.bmp");
		// data.objects[2] = l3d_read_obj("assets/models/turn_right/turn_right_ceiling.obj",
		// 							   "assets/textures/test_texture_small.bmp");
		// data.objects[3] = l3d_read_obj("assets/models/corner_bot_right/corner_bot_right_ceiling.obj",
		// 							   "assets/level0/brick_512.bmp");
		// data.objects[3] = l3d_read_obj("assets/level0/maze00.obj",
		// 							   "assets/level0/brick_512.bmp");
		data.num_objects = 1;
		int i = -1;
		// data.objects[0]->material->texture = 
		while (++i < (int)data.num_objects)
		{
		l3d_3d_object_scale(data.objects[i],
							app->window->width / 1.0,
							app->window->width / 1.0,
							app->window->width / 1.0);
		l3d_3d_object_rotate(data.objects[i], 0, 0, 0);
		l3d_3d_object_translate(data.objects[i],
			0, PLAYER_HEIGHT * 1.5, 0);
		}
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
	scene->bullet_tree = NULL;
	if (data->num_objects > 0)
	{
		ft_memmove(scene->objects, data->objects,
		sizeof(t_3d_object*) * data->num_objects);
		scene_set_triangle_refs(scene);
		l3d_kd_tree_create_or_update(&scene->bullet_tree,
			scene->triangle_ref, scene->num_triangles);
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

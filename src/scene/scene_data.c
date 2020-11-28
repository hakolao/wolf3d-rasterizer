/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:48:19 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/28 19:18:44 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** Fill assets here (3d model & texture pairs) and their corresponding
** feature keys.
** set_main_scene_assets sets these to the scene into a hashmap that the
** procedural generator can then access & copy from.
*/

static void		main_scene_data_assets_set(t_scene_data *data)
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
	// ToDo: Vesa add normal map files here: (corresponding bitflag keys below)
	data->normal_map_files[0] = NULL;
	data->normal_map_files[1] = NULL;
	data->normal_map_files[2] = NULL;
	data->normal_map_files[3] = NULL;
	data->normal_map_files[4] = NULL;
	data->normal_map_files[5] = NULL;
	data->normal_map_files[6] = NULL;
	data->normal_map_files[7] = NULL;
	data->normal_map_files[8] = NULL;
	data->normal_map_files[9] = NULL;
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
	data->num_assets_to_load = 10;
}

void		scene_main_game_data_set(t_scene_data *data)
{
	data->menu_options[0] = "Main Menu";
	data->menu_options[1] = "Exit Game";
	data->menu_option_count = 2;
	data->main_camera = new_camera();
	data->map_filename = ft_strdup("maps/bigone");
	main_scene_data_assets_set(data);
}

void		scene_main_menu_data_set(t_scene_data *data)
{
	data->menu_options[0] = "Start Game";
	data->menu_options[1] = "Settings";
	data->menu_options[2] = "Quit";
	data->menu_option_count = 3;
}

void		scene_settings_menu_data_set(t_scene_data *data)
{
	data->menu_options[0] = "1280x720";
	data->menu_options[1] = "1920x1080";
	data->menu_options[2] = "Back";
	data->menu_option_count = 3;
}

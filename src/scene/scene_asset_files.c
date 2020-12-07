/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_asset_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 20:51:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		main_scene_texture_files_set(t_scene_data *data)
{
	data->texture_files[0] = "assets/textures/lava.bmp";
	data->texture_files[1] = NULL;
	data->texture_files[2] = "assets/textures/rock.bmp";
	data->texture_files[3] = "assets/textures/rock.bmp";
	data->texture_files[4] = "assets/textures/rock.bmp";
	data->texture_files[5] = "assets/textures/rock.bmp";
	data->texture_files[6] = "assets/textures/rock.bmp";
	data->texture_files[7] = "assets/textures/rock.bmp";
	data->texture_files[8] = "assets/textures/rock.bmp";
	data->texture_files[9] = "assets/textures/rock.bmp";
	data->texture_files[10] = "assets/textures/rock.bmp";
	data->texture_files[11] = "assets/textures/rock.bmp";
	data->texture_files[12] = "assets/textures/rock.bmp";
	data->texture_files[13] = "assets/textures/rock.bmp";
	data->texture_files[14] = "assets/textures/rock.bmp";
	data->texture_files[15] = "assets/textures/rock.bmp";
}

static void		main_scene_normal_files_set(t_scene_data *data)
{
	data->normal_map_files[0] = "assets/textures/lava_normal.bmp";
	data->normal_map_files[1] = NULL;
	data->normal_map_files[2] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[3] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[4] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[5] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[6] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[7] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[8] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[9] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[10] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[11] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[12] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[13] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[14] = "assets/textures/rock_normal.bmp";
	data->normal_map_files[15] = "assets/textures/rock_normal.bmp";
}

static void		main_scene_model_files_set(t_scene_data *data)
{
	data->model_files[0] = "assets/models/room_models/floor.obj";
	data->model_files[1] = NULL;
	data->model_files[2] = "assets/models/room_models/wall_up_lp.obj";
	data->model_files[3] = "assets/models/room_models/wall_right_lp.obj";
	data->model_files[4] = "assets/models/room_models/wall_down_lp.obj";
	data->model_files[5] = "assets/models/room_models/wall_left_lp.obj";
	data->model_files[6] = "assets/models/room_models/pillar_nw.obj";
	data->model_files[7] = "assets/models/room_models/pillar_ne.obj";
	data->model_files[8] = "assets/models/room_models/pillar_se.obj";
	data->model_files[9] = "assets/models/room_models/pillar_sw.obj";
	data->model_files[10] = "assets/models/room_models/corner_filler_nw.obj";
	data->model_files[11] = "assets/models/room_models/corner_filler_ne.obj";
	data->model_files[12] = "assets/models/room_models/corner_filler_se.obj";
	data->model_files[13] = "assets/models/room_models/corner_filler_sw.obj";
	data->model_files[14] = "assets/models/room_models/ceiling.obj";
	data->model_files[15] = "assets/models/room_models/hole_in_ceiling.obj";
}

void			main_scene_data_asset_files_set(t_scene_data *data)
{
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
	data->asset_keys[10] = c_corner_nw;
	data->asset_keys[11] = c_corner_ne;
	data->asset_keys[12] = c_corner_se;
	data->asset_keys[13] = c_corner_sw;
	data->asset_keys[14] = c_ceiling;
	data->asset_keys[15] = c_ceiling_window;
	data->num_assets_to_load = 16;
	main_scene_texture_files_set(data);
	main_scene_normal_files_set(data);
	main_scene_model_files_set(data);
}

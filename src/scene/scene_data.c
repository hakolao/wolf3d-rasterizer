/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:48:19 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/05 17:55:17 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		scene_main_game_data_set(t_scene_data *data)
{
	data->menu_options[0] = "Main Menu";
	data->menu_options[1] = "Exit Game";
	data->menu_option_count = 2;
	data->main_camera = new_camera();
	data->map_filename = ft_strdup("maps/bigone");
	main_scene_data_asset_files_set(data);
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

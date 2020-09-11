/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/08 14:27:22 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		render_main_menu_ui(t_wolf3d *app)
{
	int32_t			row_height;
	int32_t			y;
	int32_t			i;
	int32_t			selected_option;

	row_height = 2 * get_relative_font_size(app, FONT_SIZE);
	y = app->main_window->height / 2 - row_height;
	selected_option = app->active_scene->selected_option;
	i = -1;
	while (++i < app->active_scene->menu_option_count)
		render_centered_text(app, (t_text_params){
			.text = app->active_scene->menu_options[i],
			.font = GAME_FONT, .blend_ratio = 1.0, .font_size = FONT_SIZE,
			.xy = (int[2]){app->main_window->width / 2, y + i * row_height},
			.text_color = selected_option == i ?
			(SDL_Color){255, 255, 255, 255} : (SDL_Color){255, 0, 0, 255}});
}

static void		render_main_game_ui(t_wolf3d *app)
{
	(void)app;
}

static void		render_debug_information(t_wolf3d *app)
{
	char	debug_info[1024];
	char	pos[64];
	char	dir[64];

	ml_vector3_to_str(app->player.pos, pos);
	ml_vector3_to_str(app->player.forward, dir);
	ft_sprintf(debug_info,
		"fps: %u\n"
		"delta time: %.2f\n"
		"pos: %s"
		"dir: %s",
		app->debug_info.fps,
		app->debug_info.avg_delta_time,
		pos,
		dir);
	// if (app->active_scene->scene_id != scene_id_main_menu)
	// 	render_debug_grid(app);
	render_text(app, (t_text_params){
		.text = debug_info, .font = DEBUG_FONT, .blend_ratio = 1.0,
		.font_size = FONT_SIZE, .xy = (int[2]){5, 5},
		.text_color = (SDL_Color){255, 255, 255, 0}});
}

void			render_ui(t_wolf3d *app)
{
	if (app->active_scene->scene_id == scene_id_main_menu)
		render_main_menu_ui(app);
	else if (app->active_scene->scene_id == scene_id_main_game)
		render_main_game_ui(app);
	if (app->is_debug)
		render_debug_information(app);
}

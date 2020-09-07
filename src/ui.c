/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/07 16:39:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		render_main_menu_ui(t_wolf3d *app)
{
	int32_t			row_height;
	int32_t			y;
	int32_t			i;
	int32_t			selected_option;

	row_height = 2 * get_font_size(app);
	y = app->main_window->height / 2 - row_height;
	selected_option = app->active_scene->selected_option;
	i = -1;
	while (++i < app->active_scene->menu_option_count)
		render_centered_text(app, app->active_scene->menu_options[i],
			selected_option == i ?
			(SDL_Color){255, 255, 255, 255} : (SDL_Color){255, 0, 0, 255},
			(int[2]){app->main_window->width / 2, y + i * row_height});
}

static void		render_main_game_ui(t_wolf3d *app)
{
	(void)app;
}

static void		render_debug_information(t_wolf3d *app)
{
	char	debug_info[256];
	char	pos[32];

	ml_vector3_to_str(app->player.pos, pos);
	ft_sprintf(debug_info,
		"fps: %u\n"
		"delta time: %.2f\n"
		"pos: %s",
		app->debug_info.fps,
		app->debug_info.avg_delta_time,
		pos);
	render_text(app, debug_info,
		(SDL_Color){255, 255, 255, 0}, (int[2]){5, 5});
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

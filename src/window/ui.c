/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 18:14:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		ui_main_menu_render(t_wolf3d *app)
{
	int32_t			row_height;
	int32_t			y;
	int32_t			i;
	int32_t			selected_option;

	row_height = 2 * FONT_SIZE;
	y = app->window->height / 2 - row_height;
	selected_option = app->active_scene->selected_option;
	i = -1;
	while (++i < app->active_scene->menu_option_count)
		window_text_render_centered(app->window, (t_text_params){
			.text = app->active_scene->menu_options[i],
			.blend_ratio = 1.0,
			.xy = (int[2]){app->window->width / 2, y + i * row_height},
			.text_color = selected_option == i ?
			(SDL_Color){255, 255, 255, 255} : (SDL_Color){255, 0, 0, 255}},
			app->window->main_font);
}

static void		ui_main_game_render(t_wolf3d *app)
{
	(void)app;
}

void			ui_render(t_wolf3d *app)
{
	if (app->active_scene->scene_id == scene_id_main_menu)
		ui_main_menu_render(app);
	else if (app->active_scene->scene_id == scene_id_main_game)
		ui_main_game_render(app);
}

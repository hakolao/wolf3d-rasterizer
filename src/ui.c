/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/25 13:09:28 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		ui_menu_render(t_wolf3d *app)
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
	l3d_line_draw(app->window->framebuffer->buffer,
		(uint32_t[2]){app->window->framebuffer->width,
			app->window->framebuffer->height},
		(int32_t[2][2]){{app->window->width / 2 - 10, app->window->height / 2},
		{app->window->width / 2 + 10, app->window->height / 2}}, 0xFFFFFFFF);
	l3d_line_draw(app->window->framebuffer->buffer,
		(uint32_t[2]){app->window->framebuffer->width,
			app->window->framebuffer->height},
		(int32_t[2][2]){{app->window->width / 2, app->window->height / 2 - 10},
		{app->window->width / 2, app->window->height / 2 + 10}}, 0xFFFFFFFF);
}

static void		framebuffer_dark_overlay(t_wolf3d *app)
{
	int32_t		i;

	i = -1;
	while (++i < app->window->framebuffer->width *
		app->window->framebuffer->height)
	{
		app->window->framebuffer->buffer[i] = l3d_color_blend_u32(
			app->window->framebuffer->buffer[i],
			0x000000FF, 0.5);
	}
}

void			ui_render(t_wolf3d *app)
{

	if (app->active_scene->scene_id == scene_id_main_menu ||
		app->active_scene->scene_id == scene_id_main_menu_settings)
		ui_menu_render(app);
	else if (app->active_scene->scene_id == scene_id_main_game)
	{
		ui_main_game_render(app);
		if (app->active_scene->is_menu_on)
		{
			framebuffer_dark_overlay(app);
			ui_menu_render(app);
		}			
	}
}

void			loading_render(t_wolf3d *app)
{
	window_text_render_centered(app->window, (t_text_params){
		.text = "Loading...",
		.blend_ratio = 1.0,
		.xy = (int32_t[2]){app->window->width / 2, app->window->height / 2},
		.text_color = (SDL_Color){255, 255, 255, 255}},
		app->window->main_font);
}

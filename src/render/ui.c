/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/26 15:43:20 by ohakola+vei      ###   ########.fr       */
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

/*
** / 2.0 because models are positioned with unitsize * 2
*/

static void		player_pos_to_grid_pos(t_wolf3d *app, t_vec2 grid_pos)
{
	grid_pos[0] = -(app->player.pos[2] / app->unit_size / 2.0) + 0.5;
	grid_pos[1] = (app->player.pos[0] / app->unit_size / 2.0) + 0.5;
}

static void		framebuffer_dark_overlay(t_framebuffer *framebuffer,
					int32_t width, int32_t height, t_vec2 pos)
{
	uint32_t	dark[width * height];
	int32_t		i;

	i = -1;
	while (++i < width * height)
		dark[i] = 0x000000FF;
	l3d_image_place(&(t_surface){.pixels = framebuffer->buffer,
			.h = framebuffer->height, .w = framebuffer->width},
		&(t_surface){.pixels = dark, .w = width, .h = height},
		(int32_t[2]){pos[0], pos[1]}, 0.5);
}

static void		minimap_render(t_wolf3d *app)
{
	t_vec2		player_grid_pos;

	player_pos_to_grid_pos(app, player_grid_pos);
	if (app->is_minimap_largened)
	{
		framebuffer_dark_overlay(app->window->framebuffer,
			app->window->width, app->window->height, (t_vec2){0, 0});
		map_minimap_render_full(app->active_scene->map,
			app->window->framebuffer, player_grid_pos);
	}
	else
	{
		framebuffer_dark_overlay(app->window->framebuffer,
			app->active_scene->map->render_size + 4,
			app->active_scene->map->render_size + 4,
			(t_vec2){app->active_scene->map->render_pos[0] - 2,
				app->active_scene->map->render_pos[1] - 2});
		map_minimap_render_partial(app->active_scene->map,
			app->window->framebuffer, player_grid_pos);
	}
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
	minimap_render(app);
}

void			ui_render(t_wolf3d *app)
{

	if (app->active_scene->scene_id == scene_id_main_menu ||
		app->active_scene->scene_id == scene_id_main_menu_settings)
		ui_menu_render(app);
	else if (app->active_scene->scene_id == scene_id_main_game)
	{
		ui_main_game_render(app);
		if (app->active_scene->is_paused)
		{
			framebuffer_dark_overlay(app->window->framebuffer,
				app->window->width, app->window->height, (t_vec2){0, 0});
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

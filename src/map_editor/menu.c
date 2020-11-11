/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 19:27:17 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/11 17:50:45 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_surface			*convert_sdl_surface_to_t_surface(SDL_Surface *src)
{
	t_surface	*dst;

	error_check(!(dst = malloc(sizeof(t_surface))),
		"Failed to malloc surface");
	error_check(!(dst->pixels = malloc(sizeof(uint32_t) *
		src->w * src->h)), "Failed to malloc surface pixels");
	ft_memcpy(dst->pixels, src->pixels,
		sizeof(uint32_t) * src->w * src->h);
	dst->w = src->w;
	dst->h = src->h;
	return (dst);
}

static void			on_menu_button_click(t_button *self, void *params)
{
	t_map_editor	*app;

	app = params;
	if (self->id == 0)
		app->selected_feature = c_floor;
	else if (self->id == 1)
		app->selected_feature = c_floor_start;
	else if (self->id == 2)
		app->selected_feature = c_wall_up;
	else if (self->id == 3)
		app->selected_feature = c_wall_right;
	else if (self->id == 4)
		app->selected_feature = c_wall_down;
	else if (self->id == 5)
		app->selected_feature = c_wall_left;
	else if (self->id == 6)
		app->selected_feature = c_clear;
}

void				map_editor_menu_create(t_map_editor *app)
{
	int32_t		i;
	const char	*options[7];
	t_button	**buttons;
	uint32_t	num_buttons;
	t_surface	**surfaces;
	t_surface	**down_surfaces;
	SDL_Surface	*tmp_surface;

	num_buttons = 7;
	options[0] = "Floor";
	options[1] = "Start";
	options[2] = "WallUp";
	options[3] = "WallRight";
	options[4] = "WallDown";
	options[5] = "WallLeft";
	options[6] = "Clear";
	error_check(!(buttons = malloc(sizeof(t_button*) * num_buttons)),
		"Failed to malloc buttons in menu creation");
	error_check(!(surfaces = malloc(sizeof(t_surface*) * num_buttons)),
		"Failed to malloc surfaces in menu creation");
	error_check(!(down_surfaces = malloc(sizeof(t_surface*) * num_buttons)),
		"Failed to malloc down_surfaces in menu creation");
	i = -1;
	while (++i < (int32_t)num_buttons)
	{
		tmp_surface = surface_from_font(app->window,
			(t_text_params){.text = options[i], .blend_ratio = 1.0,
			.text_color = (SDL_Color){255, 0, 0, 255}}, app->window->main_font);
		surfaces[i] = convert_sdl_surface_to_t_surface(tmp_surface);
		SDL_FreeSurface(tmp_surface);
		tmp_surface = surface_from_font(app->window,
			(t_text_params){.text = options[i], .blend_ratio = 1.0,
			.text_color = (SDL_Color){255, 255, 255, 255}},  app->window->main_font);
		down_surfaces[i] = convert_sdl_surface_to_t_surface(tmp_surface);
		SDL_FreeSurface(tmp_surface);
		buttons[i] = button_create(app->window, i);
		button_set_texture(buttons[i], surfaces[i], down_surfaces[i]);
		button_set_handles(buttons[i],
			on_menu_button_click, NULL);
		button_set_handle_params(buttons[i], app, NULL);
	}
	app->select_menu = button_group_create(buttons, num_buttons);
	button_group_set_space_between(app->select_menu, 5);
	// Select first feature:
	button_group_set_selector(app->select_menu, 0);
	app->select_menu->buttons[0]->on_click(app->select_menu->buttons[0], app);
	free(surfaces);
	free(down_surfaces);
}

void		map_editor_menu_render(t_map_editor *app, t_vec2 pos)
{
	if (pos[0] != app->select_menu->pos[0] ||
		pos[1] != app->select_menu->pos[1])
		button_group_update_position(app->select_menu, pos);
	button_group_render(app->select_menu);
}
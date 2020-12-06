/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:45:39 by ohakola          ###   ########.fr       */
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
		app->selected_feature = m_room;
	else if (self->id == 1)
		app->selected_feature = m_start;
	else if (self->id == 2)
		app->selected_feature = m_enemy;
	else if (self->id == 3)
		app->selected_feature = m_clear;
}

static void			set_menu_button_surfaces(t_map_editor *app,
						const char *option,
						t_surface **surface, t_surface **down_surface)
{
	SDL_Surface	*tmp_surface;

	tmp_surface = surface_from_font(app->window,
		(t_text_params){.text = option, .blend_ratio = 1.0,
		.text_color = (SDL_Color){255, 0, 0, 255}}, app->window->main_font);
	*surface = convert_sdl_surface_to_t_surface(tmp_surface);
	SDL_FreeSurface(tmp_surface);
	tmp_surface = surface_from_font(app->window,
		(t_text_params){.text = option, .blend_ratio = 1.0,
		.text_color = (SDL_Color){255, 255, 255, 255}},
		app->window->main_font);
	*down_surface = convert_sdl_surface_to_t_surface(tmp_surface);
	SDL_FreeSurface(tmp_surface);
}

void				map_editor_draw_menu_create(t_map_editor *app)
{
	int32_t		i;
	t_button	**buttons;
	t_surface	**surfaces;
	t_surface	**down_surfaces;

	error_check(!(buttons = malloc(sizeof(t_button*) * 4)), "!Merr");
	error_check(!(surfaces = malloc(sizeof(t_surface*) * 4)), "!Merr");
	error_check(!(down_surfaces = malloc(sizeof(t_surface*) * 4)), "!Merr");
	i = -1;
	while (++i < 4)
	{
		set_menu_button_surfaces(app,
			(const char*[4]){"Room", "Start", "Enemy", "Clear"}[i],
			&surfaces[i], &down_surfaces[i]);
		buttons[i] = button_create(app->window, i);
		button_set_texture(buttons[i], surfaces[i], down_surfaces[i]);
		button_set_handles(buttons[i], on_menu_button_click, NULL);
		button_set_handle_params(buttons[i], app, NULL);
	}
	app->select_menu = button_group_create(buttons, 4);
	button_group_set_space_between(app->select_menu, 5);
	button_group_set_selector(app->select_menu, 0);
	app->select_menu->buttons[0]->on_click(app->select_menu->buttons[0], app);
	free(surfaces);
	free(down_surfaces);
}

void				map_editor_menu_render(t_map_editor *app, t_vec2 pos)
{
	if (pos[0] != app->select_menu->pos[0] ||
		pos[1] != app->select_menu->pos[1])
	{
		button_group_update_position(app->select_menu, pos);
		button_group_update_position(app->save_menu,
			(t_vec2){pos[0], pos[1] + 200});
	}
	button_group_render(app->select_menu);
	button_group_render(app->save_menu);
}

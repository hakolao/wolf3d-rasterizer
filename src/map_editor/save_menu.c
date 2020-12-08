/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 01:37:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:45:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void			save_map(t_map_editor *app)
{
	int				fd;
	char			new_filename[64];
	char			filename[128];
	char			bytes[4];
	int32_t			i;

	ft_sprintf(new_filename, "maps/wolf3d_map_t%u", SDL_GetTicks());
	ft_sprintf(filename, "%s", app->filename ? app->filename : new_filename);
	if ((fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1 &&
		ft_dprintf(2, "Failed to open file %s\n", filename))
		exit(EXIT_FAILURE);
	if (write(fd, "MAP\0", 4))
	{
	}
	ft_memcpy(bytes, &app->map->size, sizeof(uint32_t));
	if (write(fd, &bytes, sizeof(uint32_t)))
	{
	}
	i = -1;
	while (++i < app->map->size * app->map->size)
	{
		ft_memcpy(bytes, &app->map->grid[i], sizeof(uint32_t));
		if (write(fd, &bytes, sizeof(uint32_t)))
		{
		}
	}
	ft_printf("Saved %s\n", filename);
	if ((fd = close(fd)) == -1 &&
		ft_dprintf(2, "Failed to close file %s\n", filename))
		exit(EXIT_FAILURE);
}

static void			on_save_menu_button_click(t_button *self, void *params)
{
	t_map_editor	*app;
	int32_t			i;

	app = params;
	if (self->id == 0)
		save_map(app);
	else if (self->id == 1)
	{
		i = -1;
		while (++i < app->map->size * app->map->size)
			app->map->grid[i] = 0;
	}
}

static void			set_save_menu_button_surfaces(t_map_editor *app,
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

void				map_editor_save_menu_create(t_map_editor *app)
{
	int32_t		i;
	const char	*options[2];
	t_button	**buttons;
	t_surface	**surfaces;
	t_surface	**down_surfaces;

	options[0] = "Save";
	options[1] = "ClearAll";
	error_check(!(buttons = malloc(sizeof(t_button*) * 2)), "!Merror");
	error_check(!(surfaces = malloc(sizeof(t_surface*) * 2)), "!Merror");
	error_check(!(down_surfaces = malloc(sizeof(t_surface*) * 2)), "!Merror");
	i = -1;
	while (++i < 2)
	{
		set_save_menu_button_surfaces(app,
			options[i], &surfaces[i], &down_surfaces[i]);
		buttons[i] = button_create(app->window, i);
		button_set_texture(buttons[i], surfaces[i], down_surfaces[i]);
		button_set_handles(buttons[i], on_save_menu_button_click, NULL);
		button_set_handle_params(buttons[i], app, NULL);
	}
	app->save_menu = button_group_create(buttons, 2);
	button_group_set_space_between(app->save_menu, 5);
	free(surfaces);
	free(down_surfaces);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 23:26:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		resize_dependent_recreate(t_map_editor *app)
{
	float	render_size;

	window_frame_recreate(app->window);
	render_size = app->window->height * 0.8;
	map_render_resize(app->map, render_size,
		(t_vec2){app->window->width / 2 - render_size / 2,
			app->window->height / 2 - render_size / 2});
	app->window->resized = false;
	while (app->window->is_hidden)
		SDL_PollEvent(NULL);
}

static void		update_mouse_grid_pos(t_map_editor *app)
{
	ml_vector2_copy((t_vec2){
		ft_floor(((float)(app->mouse.x - app->map->render_pos[0]) /
			app->map->cell_render_size)),
		ft_floor(((float)(app->mouse.y - app->map->render_pos[1]) /
			app->map->cell_render_size))},
		app->mouse_grid_pos);
}

static void		handle_feature_placement(t_map_editor *app)
{
	int32_t		i;
	uint32_t	*cell;

	if (app->mouse_grid_pos[0] < 0 ||
		app->mouse_grid_pos[0] >= app->map->size ||
		app->mouse_grid_pos[1] < 0 ||
		app->mouse_grid_pos[1] >= app->map->size)
		return ;
	cell = &app->map->grid[(int32_t)app->mouse_grid_pos[1] *
		app->map->size + (int32_t)app->mouse_grid_pos[0]];
	if (app->selected_feature == m_start)
	{
		i = -1;
		while (++i < app->map->size * app->map->size)
		{
			if (app->map->grid[i] & m_start)
				app->map->grid[i] ^= m_start;
		}
		*cell |= m_room;
		*cell |= m_start;
	}
	else if (app->selected_feature == m_clear)
		*cell = m_clear;
	else if (app->selected_feature == m_room)
		*cell |= m_room;
	update_map_cell_features(app);
}

static void		main_loop(t_map_editor *app)
{
	SDL_Event	event;

	while (app->is_running)
	{
		app->info.performance_start = SDL_GetPerformanceCounter();
		app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
		app->keyboard.state = SDL_GetKeyboardState(NULL);
		update_mouse_grid_pos(app);
		if ((app->mouse.state & SDL_BUTTON_LMASK))
			handle_feature_placement(app);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
			button_group_events_handle(app->select_menu, app->mouse, event);
			button_group_events_handle(app->save_menu, app->mouse, event);
		}
		if (app->window->resized)
			resize_dependent_recreate(app);
		window_frame_clear(app->window);
		map_editor_map_render(app);
		map_editor_menu_render(app, (t_vec2){5, 20});
		window_frame_draw(app->window);
	}
}

static void		cleanup(t_map_editor *app)
{
	if (app->filename != NULL)
		ft_strdel(&app->filename);
	map_destroy(app->map);
	button_group_destroy(app->select_menu);
	thread_pool_destroy(app->thread_pool);
	window_destroy(app->window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int				main(int argc, char **argv)
{
	t_map_editor	app;
	int32_t			size;
	int32_t			i;

	app.thread_pool = thread_pool_create(NUM_THREADS_DEFAULT);
	app.is_running = true;
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_create(&app.window, MAP_EDITOR_WIDTH, MAP_EDITOR_HEIGHT);
	map_editor_draw_menu_create(&app);
	map_editor_save_menu_create(&app);
	app.filename = NULL;
	if (argc > 1)
	{
		size = INITIAL_MAP_SIZE;
		i = -1;
		while (++i < argc)
		{
			if (ft_match(argv[i], "--size=*"))
				size = ft_atoi(argv[i] + 7);
			else if (ft_match(argv[i], "--filename=*") && app.filename == NULL)
				app.filename = ft_strdup(argv[i] + 11);
		}
		map_editor_map_init(&app, size > 0 && size <= 50 ? size : INITIAL_MAP_SIZE);
	} else
		map_editor_map_init(&app, INITIAL_MAP_SIZE);
	main_loop(&app);
	cleanup(&app);
	return (EXIT_SUCCESS);
}

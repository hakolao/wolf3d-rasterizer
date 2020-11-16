/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 18:16:02 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 15:09:36 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		resize_dependent_recreate(t_map_editor *app)
{
	window_frame_recreate(app->window);
	app->window->resized = false;
	rescale_map(app);
	while (app->window->is_hidden)
		SDL_PollEvent(NULL);
}

static void		update_mouse_grid_pos(t_map_editor *app)
{
	ml_vector2_copy((t_vec2){
		ft_floor(((float)(app->mouse.x - app->grid_pos[0]) /
			app->map->cell_render_size)),
		ft_floor(((float)(app->mouse.y - app->grid_pos[1]) /
			app->map->cell_render_size))},
		app->mouse_grid_pos);
}

static void		handle_feature_placement(t_map_editor *app)
{
	int32_t	i;

	if (app->mouse_grid_pos[0] < 0 ||
		app->mouse_grid_pos[0] >= app->map->size ||
		app->mouse_grid_pos[1] < 0 ||
		app->mouse_grid_pos[1] >= app->map->size)
		return ;
	if (app->selected_feature == c_floor_start)
	{
		i = -1;
		while (++i < app->map->size * app->map->size)
		{
			if (app->map->grid[i] & c_floor_start)
				app->map->grid[i] ^= c_floor_start;
		}
	}
	else if (app->selected_feature == c_clear)
	{
		app->map->grid[(int32_t)app->mouse_grid_pos[1] *
			app->map->size + (int32_t)app->mouse_grid_pos[0]] = 0;
		return ;
	}
	app->map->grid[(int32_t)app->mouse_grid_pos[1] *
		app->map->size + (int32_t)app->mouse_grid_pos[0]] |= app->selected_feature;
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
		if (app->mouse.state & SDL_BUTTON_LMASK)
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
		map_render(app,
			(t_vec2){(float)app->window->width / 2.0 - app->map->render_size / 2.0,
			(float)app->window->height / 2.0 - app->map->render_size / 2.0});
		map_editor_menu_render(app, (t_vec2){5, 20});
		window_frame_draw(app->window);
	}
}

static void		cleanup(t_map_editor *app)
{
	int32_t		i;
	t_surface	*image;

	if (app->filename != NULL)
		ft_strdel(&app->filename);
	i = -1;
	while (++i < app->num_images)
	{
		image = hash_map_get(app->map_images, app->image_keys[i]);
		free(image->pixels);
		free(image);
	}
	hash_map_destroy(app->map_images);
	free(app->map->grid);
	free(app->map);
	button_group_destroy(app->select_menu);
	thread_pool_destroy(app->thread_pool);
	free(app->window->buffers->framebuffer);
	free(app->window->buffers->zbuffer);
	SDL_DestroyRenderer(app->window->renderer);
	SDL_DestroyWindow(app->window->window);
	TTF_CloseFont(app->window->main_font);
	TTF_CloseFont(app->window->debug_font);
	free(app->window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int				main(int argc, char **argv)
{
	t_map_editor	app;

	app.thread_pool = thread_pool_create(NUM_THREADS);
	app.is_running = true;
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_create(&app.window, MAP_EDITOR_WIDTH, MAP_EDITOR_HEIGHT);
	map_editor_draw_menu_create(&app);
	map_editor_save_menu_create(&app);
	if (argc == 2)
	{
		app.filename = ft_strdup(argv[1]);
		init_map(&app, MAP_SIZE);
	} else
	{
		app.filename = NULL;
		init_map(&app, MAP_SIZE);
	}
	main_loop(&app);
	cleanup(&app);
	return (EXIT_SUCCESS);
}

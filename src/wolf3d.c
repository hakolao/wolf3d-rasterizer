/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/05 19:26:46 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_resize_dependent_recreate(t_wolf3d *app)
{
	app->window->resized = false;
	if (app->window->is_hidden)
	{
		while (app->window->is_hidden)
			SDL_PollEvent(NULL);
	}
	else
	{
		window_frame_recreate(app->window);
		if (app->active_scene->map)
			minimap_size_update(app);
	}
}

static void		wolf3d_main_loop(t_wolf3d *app)
{

	while (app->is_running)
	{
		app->info.performance_start = SDL_GetPerformanceCounter();
		if (app->window->resized)
			wolf3d_resize_dependent_recreate(app);
		window_frame_clear(app->window);
		if (app->is_loading)
		{
			loading_render(app);
			window_frame_draw(app->window);
			wolf3d_debug_info_capture(app);
			continue ;
		}
		events_handle(app);
		if (app->is_loading)
			continue ;
		wolf3d_render(app);
		if (app->is_debug)
			wolf3d_debug_info_render(app);
		window_frame_draw(app->window);
		wolf3d_debug_info_capture(app);
	}
}

void		wolf3d_init(t_wolf3d *app)
{
	app->active_scene = NULL;
	app->is_running = true;
	app->is_debug = true;
	app->is_loading = true;
	app->is_minimap_largened = false;
	app->unit_size = app->window->width;
	app->next_scene_id = scene_id_main_menu;
	app->is_normal_map = false;
	scene_next_select(app);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	thread_pool_destroy(app->thread_pool);
	scene_destroy(app->active_scene);
	window_destroy(app->window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void			wolf3d_run(t_wolf3d *app)
{
	int32_t	cpu_count;

	cpu_count = SDL_GetCPUCount();
	app->thread_pool = thread_pool_create(
		cpu_count >= NUM_THREADS_DEFAULT ? cpu_count : NUM_THREADS_DEFAULT);
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_create(&app->window, WIDTH, HEIGHT);
	wolf3d_init(app);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}

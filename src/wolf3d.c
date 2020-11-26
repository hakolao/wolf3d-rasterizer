/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/26 12:40:13 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_resize_dependent_recreate(t_wolf3d *app)
{
	window_frame_recreate(app->window);
	app->window->resized = false;
	while (app->window->is_hidden)
		SDL_PollEvent(NULL);
}

static void		wolf3d_main_loop(t_wolf3d *app)
{

	while (app->is_running)
	{
		app->info.performance_start = SDL_GetPerformanceCounter();
		if (app->window->resized)
			wolf3d_resize_dependent_recreate(app);
		window_frame_clear(app->window);
		handle_events(app);
		if (app->is_loading)
		{
			loading_render(app);
			window_frame_draw(app->window);
			wolf3d_debug_info_capture(app);
			continue ;
		}
		else
		{
			wolf3d_render(app);
			if (app->is_debug)
				wolf3d_debug_info_render(app);
			window_frame_draw(app->window);
			wolf3d_debug_info_capture(app);
		}
	}
}

void		wolf3d_init(t_wolf3d *app)
{
	app->active_scene = NULL;
	app->is_running = true;
	app->is_debug = true;
	app->is_loading = false;
	app->unit_size = app->window->width;
	init_player(app, (t_vec3){0, 0, 0});
	app->next_scene_id = scene_id_main_menu;
	select_next_scene(app);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	thread_pool_destroy(app->thread_pool);
	l3d_framebuffer_destroy(app->window->framebuffer);
	destroy_scene(app->active_scene);
	SDL_DestroyRenderer(app->window->renderer);
	SDL_DestroyWindow(app->window->window);
	TTF_CloseFont(app->window->main_font);
	TTF_CloseFont(app->window->debug_font);
	free(app->window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void			wolf3d_run(t_wolf3d *app)
{
	app->thread_pool = thread_pool_create(NUM_THREADS);
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_create(&app->window, WIDTH, HEIGHT);
	wolf3d_init(app);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}

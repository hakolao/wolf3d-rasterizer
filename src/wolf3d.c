/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/23 14:24:11 by ohakola+vei      ###   ########.fr       */
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
	SDL_Event	event;

	while (app->is_running)
	{
		app->info.performance_start = SDL_GetPerformanceCounter();
		// Needed to ensure mouse events work right...:S
		if (SDL_GetMouseFocus() == app->window->window)
			SDL_WarpMouseInWindow(app->window->window,
				app->window->width / 2, app->window->height / 2);
		if (app->window->resized)
			wolf3d_resize_dependent_recreate(app);
		window_frame_clear(app->window);
		// Show loading screen when assets are loading (in scene selection)
		if (app->is_loading)
		{
			loading_render(app);
			window_frame_draw(app->window);
			wolf3d_debug_info_capture(app);
			continue ;
		}
		mouse_state_set(app);
		mouse_state_handle(app);
		keyboard_state_set(app);
		keyboard_state_handle(app);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_g)
				app->is_debug = !app->is_debug;
			if (app->active_scene->scene_id == scene_id_main_menu)
				main_menu_event_handle(app, event);
			if (event.type == SDL_MOUSEMOTION)
				mouse_motion_handle(app, event);
		}
		wolf3d_render(app);
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
	app->is_loading = false;
	init_player(app, (t_vec3){0, 0, 0});
	app->next_scene_id = scene_id_main_menu;
	select_next_scene(app);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	thread_pool_destroy(app->thread_pool);
	l3d_render_buffers_destroy(app->window->framebuffer);
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

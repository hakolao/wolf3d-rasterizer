/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 18:16:02 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/09 19:34:21 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		resize_dependent_recreate(t_map_editor *app)
{
	window_frame_recreate(app->window);
	app->window->resized = false;
	while (app->window->is_hidden)
		SDL_PollEvent(NULL);
}

static void		main_loop(t_map_editor *app)
{
	SDL_Event	event;

	while (app->is_running)
	{
		app->info.performance_start = SDL_GetPerformanceCounter();
		app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
		app->keyboard.state = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		}
		if (app->window->resized)
			resize_dependent_recreate(app);
		window_frame_clear(app->window);
		//Render here
		map_editor_menu_render(app, (t_vec2){10, 20});
		window_frame_draw(app->window);
	}
}

static void		cleanup(t_map_editor *app)
{
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

int				main(void)
{
	t_map_editor	app;
	
	app.thread_pool = thread_pool_create(NUM_THREADS);
	app.is_running = true;
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_create(&app.window);
	main_loop(&app);
	cleanup(&app);
	return (EXIT_SUCCESS);
}
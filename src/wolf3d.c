/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 14:05:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_main_loop(t_wolf3d *app)
{
	SDL_Event event;
	app->is_running = true;
	while (app->is_running) {
		// Uint32 tick = SDL_GetTicks();
		app->starting_tick = SDL_GetTicks();
		while (SDL_PollEvent(&event))\
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		}
		draw_frame(app);
		// ft_printf("time between frames: %d seconds or %d ms\n",
		// 		  (SDL_GetTicks() - tick) / 1000, SDL_GetTicks() - tick);
	}
}

void		wolf3d_init(t_wolf3d *app)
{
	// (void)app;
	init_app(app);
	app->active_scene = new_scene(app, NULL);
	init_player(app);
	
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	destroy_scene(app);
	SDL_DestroyRenderer(app->main_window->renderer);
	SDL_DestroyWindow(app->main_window->window);
	free(app->main_window);
	IMG_Quit();
	SDL_Quit();
}

void			wolf3d_run(t_wolf3d *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	main_window_init(app);
	wolf3d_init(app);
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}
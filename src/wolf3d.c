/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 16:54:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_main_loop(t_wolf3d *app)
{
	SDL_Event	event;
	uint32_t	tick;

	app->is_running = true;
	draw_frame(app);
	while (app->is_running) {
		tick = SDL_GetTicks();
		while (SDL_PollEvent(&event))\
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		}
		// draw_frame(app);
		// ft_printf("time between frames: %d seconds or %d ms\n",
		// 		  (SDL_GetTicks() - tick) / 1000, SDL_GetTicks() - tick);
		// cap_framerate(tick);
	}
}

void		wolf3d_init(t_wolf3d *app)
{
	app->start_time = SDL_GetTicks();
	app->active_scene = new_scene(app, NULL);
	init_player(app->active_scene);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	destroy_scene(app->active_scene);
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
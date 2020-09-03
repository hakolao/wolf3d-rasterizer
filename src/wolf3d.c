/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 13:08:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

uint32_t		g_time_since_start;

static void		wolf3d_main_loop(t_wolf3d *app)
{
	SDL_Event	event;

	app->is_running = true;
	draw_frame(app);
	while (app->is_running) {
		g_time_since_start = SDL_GetTicks();
		while (SDL_PollEvent(&event))\
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		}
		draw_frame(app);
		cap_framerate();
	}
}

void		wolf3d_init(t_wolf3d *app)
{
	app->active_scene = new_scene(app, NULL);
	init_player(app->active_scene);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	destroy_scene(app->active_scene);
	SDL_DestroyRenderer(app->main_window->renderer);
	SDL_DestroyWindow(app->main_window->window);
	free(app->main_window);
	TTF_CloseFont(app->main_window->font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void			wolf3d_run(t_wolf3d *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	main_window_init(app);
	wolf3d_init(app);
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}

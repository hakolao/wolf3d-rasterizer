/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:51:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_main_loop(t_wolf3d *app)
{
	app->is_running = true;
	while (app->is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event))\
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		}
		draw_frame(app);
	}
}

static void		wolf3d_init(t_wolf3d *app)
{
	(void)app;
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	(void)app;
}

void			wolf3d_run(t_wolf3d *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	window_init(app);
	wolf3d_init(app);
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}
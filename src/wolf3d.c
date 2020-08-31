/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 18:03:55 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_main_loop(t_wolf3d *app)
{
	SDL_Event event;
	app->is_running = true;
	while (app->is_running) {
		app->starting_tick = SDL_GetTicks();
		while (SDL_PollEvent(&event))\
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		// 	if (event.type == SDL_KEYDOWN)
		// 	{
		// 		if (event.key.keysym.sym == SDLK_w)
		// 			move_player(&app->player, forward);
		// 		else if (event.key.keysym.sym == SDLK_s)
		// 			move_player(&app->player, backward);
		// 		else if (event.key.keysym.sym == SDLK_a)
		// 			move_player(&app->player, strafe_left);
		// 		else if (event.key.keysym.sym == SDLK_d)
		// 			move_player(&app->player, strafe_right);
		// 	}
		}
		// draw_frame(app);
		// cap_framerate(app->starting_tick);
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
	// ToDo: Free pixels
	free(app->main_window);
	//free objects
	//free camera
	//free scene
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
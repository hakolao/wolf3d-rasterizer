/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 19:23:51 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void 	main_menu_event_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_UP)
			app->active_scene->selected_option--;
		else if (event.key.keysym.sym == SDLK_DOWN)
			app->active_scene->selected_option++;
		if (app->active_scene->selected_option >=
			app->active_scene->menu_option_count)
			app->active_scene->selected_option = 0;
		if (app->active_scene->selected_option < 0)
			app->active_scene->selected_option =
				app->active_scene->menu_option_count - 1;
		if (event.key.keysym.sym == SDLK_RETURN)
		{
			if (app->active_scene->selected_option == menu_action_start_game)
				set_active_scene(app, scene_id_main_game);
			else if (app->active_scene->selected_option == menu_action_quit_game)
				app->is_running = false;
		}
	}
}

static void		wolf3d_main_loop(t_wolf3d *app)
{
	SDL_Event	event;

	app->is_running = true;
	draw_frame(app);
	while (app->is_running)
	{
		app->time_since_start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
			if (app->active_scene->scene_id == scene_id_main_menu)
				main_menu_event_handle(app, event);
		}
		draw_frame(app);
		app->delta_time = SDL_GetTicks() - app->time_since_start;
		cap_framerate(app);
	}
}

void		wolf3d_init(t_wolf3d *app)
{
	app->active_scene = NULL;
	init_player(app);
	set_active_scene(app, scene_id_main_menu);
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

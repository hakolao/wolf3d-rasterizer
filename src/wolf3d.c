/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 18:27:53 by ohakola          ###   ########.fr       */
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

void 			player_action_handle(t_wolf3d *app, SDL_Event event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		rotate_player_vertical(app, (float)event.motion.yrel * 0.3);
		rotate_player_horizontal(app, -(float)event.motion.xrel * 0.3);
	}
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_w)
			move_player(app, move_forward);
		else if (event.key.keysym.sym == SDLK_a)
			move_player(app, move_strafe_left);
		else if (event.key.keysym.sym == SDLK_s)
			move_player(app, move_backward);
		else if (event.key.keysym.sym == SDLK_d)
			move_player(app, move_strafe_right);
		else if (event.key.keysym.sym == SDLK_q)
			l3d_3d_object_rotate(app->active_scene->objects[0], 0, -1, 0);
		else if (event.key.keysym.sym == SDLK_e)
			l3d_3d_object_rotate(app->active_scene->objects[0], 0, 1, 0);
		else if (event.key.keysym.sym == SDLK_z)
			l3d_3d_object_rotate(app->active_scene->objects[0], -1, 0, 0);
		else if (event.key.keysym.sym == SDLK_c)
			l3d_3d_object_rotate(app->active_scene->objects[0], 1, 0, 0);
	}
}

static void		wolf3d_resize_dependent_recreate(t_wolf3d *app)
{
	window_frame_recreate(app->window);
	app->window->resized = false;
	while (app->window->is_hidden)
		SDL_PollEvent(NULL);
}

static void		wolf3d_debug_info_capture(t_wolf3d *app)
{
	app->info.performance_end = SDL_GetPerformanceCounter();
	app->info.delta_time =
		(app->info.performance_end - app->info.performance_start) * 1000.0 /
		SDL_GetPerformanceFrequency();
	app->info.fps = window_framerate_capture(app->info.delta_time);
}

static void		wolf3d_main_loop(t_wolf3d *app)
{
	SDL_Event	event;

	while (app->is_running)
	{
		app->info.performance_start = SDL_GetPerformanceCounter();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_g)
				app->is_debug = !app->is_debug;
			if (app->active_scene->scene_id == scene_id_main_menu)
				main_menu_event_handle(app, event);
			else
				player_action_handle(app, event);
		}
		if (app->window->resized)
			wolf3d_resize_dependent_recreate(app);
		window_frame_clear(app->window);
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
	init_player(app);
	set_active_scene(app, scene_id_main_menu);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	free(app->window->framebuffer);
	free(app->window->zbuffer);
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
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	error_check(TTF_Init() == -1, TTF_GetError());
	window_create(&app->window);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	wolf3d_init(app);
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}

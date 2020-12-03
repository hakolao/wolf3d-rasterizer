/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 18:17:51 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/03 23:43:34 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			wolf3d_debug_info_render(t_wolf3d *app)
{
	char	debug_info[1024];
	// char	pos[64];
	// char	grid_pos[64];
	// char	dir[64];

	// if (app->active_scene->scene_id == scene_id_main_game)
	// {
	// 	ml_vector3_to_str(app->player.pos, pos);
	// 	ml_vector3_to_str(app->player.forward, dir);
	// 	ml_vector2_to_str(app->player.grid_pos, grid_pos);
	// 	ft_sprintf(debug_info,
	// 		"fps: %u\ndelta time: %u\n"
	// 		"pos: %sdir: %sgrid_pos:%s",
	// 		app->info.fps, app->info.delta_time, pos, dir, grid_pos);
	// }
	// else
	ft_sprintf(debug_info, "fps: %u\ndelta time: %u",
	app->info.fps, app->info.delta_time);
	window_text_render(app->window, (t_text_params){
		.text = debug_info, .blend_ratio = 1.0,
		.xy = (int[2]){5, 5},
		.text_color = (SDL_Color){255, 255, 255, 0}},
		app->window->debug_font);
}

void			wolf3d_debug_info_capture(t_wolf3d *app)
{
	app->info.performance_end = SDL_GetPerformanceCounter();
	app->info.delta_time =
		(app->info.performance_end - app->info.performance_start) * 1000.0 /
		SDL_GetPerformanceFrequency();
	app->info.fps = window_framerate_capture(app->info.delta_time);
}

uint64_t		wolf3d_performance_counter_start()
{
	return (SDL_GetPerformanceCounter());
}

void			wolf3d_performance_counter_end(uint64_t start_time,
					char *task_name, float delta_limit)
{
	float			delta_time;

	delta_time = (float)(SDL_GetPerformanceCounter() -
		start_time) * 1000.0 /
		(float)SDL_GetPerformanceFrequency();
	if (delta_limit == 0.0 || delta_time > delta_limit)
		ft_printf("%s: Profiler time: %f\n", task_name, delta_time);
}

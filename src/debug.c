/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 03:06:51 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** If you need to debug how many triangles are passed to rasterizer:
** ft_sprintf(debug_info, "fps: %u\ndelta time: %u\n"
** "scene triangles: %d\n"
** "triangles for rasterizer: %d",
** app->info.fps, app->info.delta_time,
** app->active_scene->num_triangles +
** 	app->active_scene->skybox[0]->num_triangles * 6,
** app->triangles_in_view);
*/

void			wolf3d_debug_info_render(t_wolf3d *app)
{
	char	debug_info[1024];

	ft_sprintf(debug_info, "fps: %u\ndelta time: %u",
		app->info.fps, app->info.delta_time);
	window_text_render(app->window, (t_text_params){
		.text = debug_info, .blend_ratio = 1.0, .xy = (int[2]){5, 5},
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

uint64_t		wolf3d_performance_counter_start(void)
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

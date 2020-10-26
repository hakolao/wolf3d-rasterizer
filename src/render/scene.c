/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 17:10:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/20 17:41:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		triangle_thread_work(void *params)
{
	t_triangle_work	*work;

	work = params;
	render_triangle(work->app, work->triangle);
	free(work);
}

static void		render_active_scene(t_wolf3d *app)
{
	int				i;
	int				j;
	t_triangle_work	*work;

	i = -1;
	j = -1;
	while (++i < (int)app->active_scene->num_objects)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
		{
			error_check(!(work = malloc(sizeof(t_triangle_work))), 
				"Failed to malloc triangle work");
			work->app = app;
			work->triangle = app->active_scene->objects[i]->triangles + j;
			thread_pool_add_work(app->thread_pool,
				triangle_thread_work, (void*)work);
		}
	}
	thread_pool_wait(app->thread_pool);
}

void			wolf3d_render(t_wolf3d *app)
{	
	if (app->active_scene->main_camera != NULL)
		render_active_scene(app);
	ui_render(app);
}

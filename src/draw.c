/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/27 16:21:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	render_active_scene_objects(t_wolf3d *app)
{
	int		i;
	int		j;

	i = -1;
	while (++i < app->active_scene->object_count)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
			render_triangle(app,
				&app->active_scene->objects[i]->triangles[j], NULL,
				app->active_scene->main_camera);	
	}
}

void		update_frame_buffer(t_wolf3d *app)
{
	ft_memset(app->main_window->framebuffer, 0,
		app->main_window->width * app->main_window->height * sizeof (uint32_t));
	ft_memset(app->main_window->rbuffer, 0, sizeof(float) * WIDTH * HEIGHT);
	if (app->active_scene->main_camera != NULL)
		render_active_scene_objects(app);
	render_ui(app);
}

void		draw_frame(t_wolf3d *app)
{

	if (app->main_window->resized)
	{
		recreate_frame(app);
		if (app->active_scene->main_camera != NULL)
			update_camera(app);
		app->main_window->resized = false;
		while (app->main_window->is_hidden)
			SDL_PollEvent(NULL);
	}
	SDL_LockTexture(app->main_window->frame, NULL,
		(void**)&app->main_window->framebuffer,
		&app->main_window->pitch);
	update_frame_buffer(app);
	SDL_UnlockTexture(app->main_window->frame);
	SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->main_window->renderer);
}

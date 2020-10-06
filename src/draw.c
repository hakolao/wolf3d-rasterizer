/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/05 14:44:27 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** Set 4 colors at a time for speed
*/

static void	clear_frame(t_wolf3d *app)
{
	int			i;
	uint32_t	color;

	i = 0;
	color = 0x000000FF;
	while (i < app->main_window->width * app->main_window->height)
	{
		app->main_window->framebuffer[i] = color;
		app->main_window->framebuffer[i + 1] = color;
		app->main_window->framebuffer[i + 2] = color;
		app->main_window->framebuffer[i + 3] = color;
		app->main_window->rbuffer[i] = color;
		app->main_window->rbuffer[i + 1] = color;
		app->main_window->rbuffer[i + 2] = color;
		app->main_window->rbuffer[i + 3] = color;
		i += 4;
	}
}

static void	render_active_scene(t_wolf3d *app)
{
	int		i;
	int		j;

	i = -1;
	while (++i < (int)app->active_scene->num_objects)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
		{
			render_triangle(app, &app->active_scene->objects[i]->triangles[j]);
			// t_vertex *vertex = app->active_scene->objects[i]->triangles[j].vtc[0];
			// ml_vector3_print(vertex->pos);
		}
	}
}

void		update_frame_buffer(t_wolf3d *app)
{
	clear_frame(app);
	
	if (app->active_scene->main_camera != NULL)
		render_active_scene(app);
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

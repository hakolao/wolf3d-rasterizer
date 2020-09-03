/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 19:27:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Converts pixel position from screen coordinates to frame buffer index
*/

int		screen_to_frame_coords(uint32_t width, int x, int y)
{
	return (y * width + x);
}

void		render_object(t_object *object, t_wolf3d *app)
{
	int		i;
	t_mesh	*mesh;
	t_scene	*scene;

	scene = object->parent_scene;
	mesh = object->mesh;
	i = -1;
	// render_mesh()
	(void)app;
}

void		render_active_scene(t_wolf3d *app)
{
	Uint32		i;

	i = -1;
	t_vertex	vtxa;
	t_vertex	vtxb;
	t_vertex	vtxc;
	t_triangle	triangle;

	ml_vector3_copy((t_vec3){app->active_scene->main_camera->screen_dist, 50, 50}, vtxa.position);
	ml_vector3_copy((t_vec3){app->active_scene->main_camera->screen_dist, -50, -50}, vtxb.position);
	ml_vector3_copy((t_vec3){app->active_scene->main_camera->screen_dist, 25, 25}, vtxc.position);
	triangle.vtc[0] = &vtxa;
	triangle.vtc[1] = &vtxb;
	triangle.vtc[2] = &vtxc;
	render_triangle(app, &triangle, NULL, app->active_scene->main_camera);
	return ;
}

void		update_frame_buffer(t_wolf3d *app)
{
	ft_memset(app->main_window->framebuffer, 0,
		app->main_window->width * app->main_window->height * sizeof (uint32_t));
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

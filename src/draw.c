/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 15:04:54 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Converts pixel position from screen coordinates to frame buffer index
*/

int		screen_to_frame_coords(t_scene *scene, int x, int y)
{
	return (y * scene->main_window->width + x);
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

void		render_scene(t_wolf3d *app, t_scene *scene)
{
	Uint32		i;

	i = -1;
	t_vertex	vtxa;
	t_vertex	vtxb;
	t_vertex	vtxc;
	t_triangle	triangle;

	ml_vector3_copy((t_vec3){scene->main_camera->screen_dist, 200, 200}, vtxa.position);
	ml_vector3_copy((t_vec3){scene->main_camera->screen_dist, -200, -200}, vtxb.position);
	ml_vector3_copy((t_vec3){scene->main_camera->screen_dist, 100, 100}, vtxc.position);
	triangle.vtc[0] = &vtxa;
	triangle.vtc[1] = &vtxb;
	triangle.vtc[2] = &vtxc;
	render_triangle(&triangle, NULL, scene->main_camera);
	ft_memcpy(app->main_window->framebuffer, scene->main_camera->framebuffer,
		sizeof(uint32_t) * app->main_window->width * app->main_window->height);
	return ;
}

void		update_frame(t_wolf3d *app)
{
	render_scene(app, app->active_scene);
}

void		draw_frame(t_wolf3d *app)
{
	if (app->main_window->resized)
	{
		// Do what must be done after resize using app->main_window->width & height
		app->main_window->resized = false;
	}
	SDL_LockTexture(app->main_window->frame, NULL,
		(void**)&app->main_window->framebuffer,
		&app->main_window->pitch);
	update_frame(app);
	SDL_UnlockTexture(app->main_window->frame);
	SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->main_window->renderer);
}

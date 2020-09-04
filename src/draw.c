/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 16:57:06 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Converts pixel position from screen coordinates to frame buffer index
*/

int		screen_to_frame_coords(t_scene *scene, int x, int y)
{
	// ft_printf("framecoords = %d\n", y * scene->main_window->width + x);
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

void		render_scene(t_scene *scene)
{
	Uint32		i;

	i = -1;
	t_vertex	*vtxa;
	t_vertex	*vtxb;
	t_vertex	*vtxc;
	t_triangle	triangle;

	vtxa = (t_vertex*)malloc(sizeof(t_vertex));
	vtxb = (t_vertex*)malloc(sizeof(t_vertex));
	vtxc = (t_vertex*)malloc(sizeof(t_vertex));

	// vtxa->position[0] = scene->main_camera->screen_dist;
	// vtxa.position[1] = 25.0;
	// vtxa.position[1] = -50.0;

	// vtxb.position[0] = scene->main_camera->screen_dist;
	// vtxb.position[1] = -50.0;
	// vtxb.position[2] = -50.0;

	// vtxc.position[0] = scene->main_camera->screen_dist;
	// vtxc.position[1] = 25.0;
	// vtxc.position[2] = 25.0;

	ml_vector3_copy((t_vec3){scene->main_camera->screen_dist, 25.0, -50.0}, vtxa->position);
	ml_vector3_copy((t_vec3){scene->main_camera->screen_dist, -50.0, -50.0}, vtxb->position);
	ml_vector3_copy((t_vec3){scene->main_camera->screen_dist, 25.0, 25.0}, vtxc->position);
	ft_printf("----------\n");
	ml_vector3_print(vtxa->position);
	ml_vector3_print(vtxb->position);
	ml_vector3_print(vtxc->position);
	triangle.vtc[0] = vtxa;
	triangle.vtc[1] = vtxb;
	triangle.vtc[2] = vtxc;
	ft_printf("----------\n");
	ml_vector3_print(triangle.vtc[0]->position);
	ml_vector3_print(triangle.vtc[1]->position);
	ml_vector3_print(triangle.vtc[2]->position);
	ft_printf("----------\n");
	render_triangle(&triangle, NULL, scene->main_camera);
	return ;
}

void		update_frame_buffer(t_wolf3d *app)
{
	render_scene(app->active_scene);
	render_scene_ui(app->active_scene);
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
	update_frame_buffer(app);
	SDL_UnlockTexture(app->main_window->frame);
	SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->main_window->renderer);
}

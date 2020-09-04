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

static void		render_grid(t_wolf3d *app)
{
	int32_t i;

	i = 0;
	while (i < app->main_window->height * app->main_window->width)
	{
		if (i % 10 == 0)
		{
			app->main_window->framebuffer[i] = 0xaa9999ff;
		}
		if (i % (app->main_window->width * 10) < app->main_window->width)
		{
			app->main_window->framebuffer[i] = 0xaa9999ff;
		}
		if (i / app->main_window->width == app->main_window->height / 2)
			app->main_window->framebuffer[i] = 0xffff00ff;
		if (i % app->main_window->width == app->main_window->width / 2)
			app->main_window->framebuffer[i] = 0xff00ffff;
	i++;
	}
}

void		render_active_scene(t_wolf3d *app)
{
	Uint32		i;
	i = -1;
	t_vec3 		v1 = {app->active_scene->main_camera->screen_dist, 20.0, 0.0};
	t_vec3		v2 = {app->active_scene->main_camera->screen_dist, 20.0, -20.0};
	t_vec3		v3 = {app->active_scene->main_camera->screen_dist, -50.0, -50.0};
	//v1.position causes 2 times the offset of each respective coordinate for v2 and v3
	//so there is a multiplier somewhere and a sign change somewhere
	t_vertex	*vtxa;
	t_vertex	*vtxb;
	t_vertex	*vtxc;
	// t_vertex *vtxa2;
	// t_vertex *vtxb2;
	// t_vertex *vtxc2;
	t_triangle	triangle;
	// t_triangle 	triangle2;
	t_vec3		e1;
	t_vec3		e2;
	// t_vec3		e12;
	// t_vec3		e22;
	vtxa = (t_vertex*)malloc(sizeof(t_vertex));
	vtxb = (t_vertex*)malloc(sizeof(t_vertex));
	vtxc = (t_vertex *)malloc(sizeof(t_vertex));
	// vtxa2 = (t_vertex *)malloc(sizeof(t_vertex));
	// vtxb2 = (t_vertex *)malloc(sizeof(t_vertex));
	// vtxc2 = (t_vertex *)malloc(sizeof(t_vertex));
	vtxb->position[1] -= vtxa->position[1];
	vtxb->position[2] -= vtxa->position[2];
	vtxc->position[1] -= vtxa->position[1];
	vtxc->position[2] -= vtxa->position[2];
	ft_memset(vtxa, 0, sizeof(t_vertex));
	ft_memset(vtxb, 0, sizeof(t_vertex));
	ft_memset(vtxc, 0, sizeof(t_vertex));
	ml_vector3_copy(v1, vtxa->position);
	ml_vector3_copy(v2, vtxb->position);
	ml_vector3_copy(v3, vtxc->position);
	ml_vector3_sub(vtxa->position, vtxb->position, e1);
	ml_vector3_sub(vtxa->position, vtxc->position, e2);
	ml_vector3_cross(e1, e2, triangle.normal);
	// ml_vector3_print(triangle.normal);
	triangle.vtc[0] = vtxa;
	triangle.vtc[1] = vtxb;
	triangle.vtc[2] = vtxc;

	// ml_vector3_copy((t_vec3){app->active_scene->main_camera->screen_dist, 0.0, 0.0}, vtxa2->position);
	// ml_vector3_copy((t_vec3){app->active_scene->main_camera->screen_dist, 0.0, -200.0}, vtxb2->position);
	// ml_vector3_copy((t_vec3){app->active_scene->main_camera->screen_dist, -50.0, -200.0}, vtxc2->position);
	// ml_vector3_sub(vtxa2->position, vtxb2->position, e12);
	// ml_vector3_sub(vtxa2->position, vtxc2->position, e22);
	// ml_vector3_cross(e1, e2, triangle2.normal);
	// ml_vector3_print(triangle2.normal);
	// triangle2.vtc[0] = vtxa2;
	// triangle2.vtc[1] = vtxb2;
	// triangle2.vtc[2] = vtxc2;
	// ft_printf("frame\n");
	render_triangle(app, &triangle, NULL, app->active_scene->main_camera);
	// render_triangle(app, &triangle2, NULL, app->active_scene->main_camera);
	render_grid(app);
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

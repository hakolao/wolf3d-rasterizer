/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/07 23:05:03 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Converts pixel position from screen coordinates to frame buffer index
**	Left top corner is considered (0,0) and bottom right (width, height)
*/

int		screen_to_frame_coords(uint32_t width, uint32_t height, int x, int y)
{
	x *= -1;
	y *= -1;
	x += width;
	y += height;
	return (x + y * width);
}

static void		init_triangle(t_triangle *triangle,
				t_vec3 pos_a, t_vec3 pos_b, t_vec3 pos_c)
{
	t_vertex	vtxa;
	t_vertex	vtxb;
	t_vertex	vtxc;
	t_vec3		e1;
	t_vec3		e2;

	ml_vector3_copy(pos_a, vtxa.position);
	ml_vector3_copy(pos_b, vtxb.position);
	ml_vector3_copy(pos_c, vtxc.position);
	ml_vector3_sub(vtxb.position, vtxa.position, e1);
	ml_vector3_sub(vtxc.position, vtxa.position, e2);
	ml_vector3_cross(e1, e2, triangle->normal);
	triangle->vtc[0] = &vtxa;
	triangle->vtc[1] = &vtxb;
	triangle->vtc[2] = &vtxc;
	ml_vector3_copy(e1, triangle->ab);
	ml_vector3_copy(e2, triangle->ac);
}


static void	render_objects(t_wolf3d *app, t_object **objects, int32_t object_count)
{
	int		i;
	int		j;

	i = -1;
	while (++i < object_count)
	{
		j = -1;
		while (++j < objects[i]->mesh->triangle_count)
			render_triangle(app, objects[i]->mesh->triangles[j], NULL,
				app->active_scene->main_camera);
	}
}

void		render_active_scene(t_wolf3d *app)
{
	t_triangle	triangle;
	t_mesh		mesh;
	t_object	o;
	t_object	*object;
	t_object	**objects;

	o.mesh = &mesh;
	object = &o;
	objects = &object;
	mesh.triangle_count = 1;
	mesh.triangles = (t_triangle*[1]){&triangle};
	init_triangle(&triangle,
		(t_vec3){app->active_scene->main_camera->screen_dist, 20.0, 40.0},
		(t_vec3){app->active_scene->main_camera->screen_dist, 20.0, -40.0},
		(t_vec3){app->active_scene->main_camera->screen_dist, -50.0, -50.0});
	render_objects(app, objects, 1);
}

void		update_frame_buffer(t_wolf3d *app)
{
	ft_memset(app->main_window->framebuffer, 0,
		app->main_window->width * app->main_window->height * sizeof (uint32_t));
	if (app->active_scene->main_camera != NULL)
	{
		render_active_scene(app);
		if (app->is_debug)
			render_debug_grid(app);
	}
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

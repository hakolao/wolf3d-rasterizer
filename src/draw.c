/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 18:38:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Converts pixel position from screen coordinates to frame buffer index
** ToDo: Consider dynamic width
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

void	copy_frame(Uint32 *dst, Uint32 *src, t_wolf3d *app)
{
	int		i;

	i = -1;
	while (++i < app->main_window->width * app->main_window->height)
	{
		dst[i] = src[i];
	}
}

void		render_scene(t_wolf3d *app, t_scene *scene)
{
	Uint32		i;

	i = -1;
	t_vertex	vtxa;
	t_vertex	vtxb;
	t_vertex	vtxc;
	t_vec3		pos;
	ml_vec3_set(pos, scene->main_camera->screen_dist, WIDTH / 2, HEIGHT / 2);
	ml_vector3_copy(vtxa.position, pos);
	ml_vec3_set(pos, scene->main_camera->screen_dist, WIDTH / 2 - 50, HEIGHT / 2 - 50);
	ml_vector3_copy(vtxb.position, pos);
	ml_vec3_set(pos, scene->main_camera->screen_dist, WIDTH / 2 + 50, HEIGHT / 2 + 50);
	ml_vector3_copy(vtxc.position, pos);
	t_triangle *triangle;
	triangle = (t_triangle*)malloc(sizeof(t_triangle));
	triangle->vtc[0] = (t_vertex*)malloc(sizeof(t_vertex));
	triangle->vtc[1] = (t_vertex*)malloc(sizeof(t_vertex));
	triangle->vtc[2] = (t_vertex*)malloc(sizeof(t_vertex));
	ml_vector3_copy(vtxa.position, triangle->vtc[0]->position);
	ml_vector3_copy(vtxc.position, triangle->vtc[1]->position);
	ml_vector3_copy(vtxb.position, triangle->vtc[2]->position);
	render_triangle(triangle, NULL, scene->main_camera);
	copy_frame(app->main_window->framebuffer, scene->main_camera->framebuffer,
				app);
	(void)app;
	(void)scene;
	// while (i++ < scene->object_count)
	// {
	// 	render_object(scene->objects[i], app);
	// }
	//RENDER UI

	// size_t	i;
	// size_t	j;
	// t_vec4	screen_pos;

	// i = -1;
	// ft_memset(&screen_pos, 0, sizeof(t_vec4));
	// while (++i < app->active_scene.object_count)
	// {
	// 	j = -1;
	// 	while (++j < app->active_scene.objects[i].vertex_count)
	// 	{
	// 		camera_transform(&app->player.camera,
	// 			app->active_scene.objects[i].vertices[j].position, screen_pos);
	// 		// Check it's in front of camera
	// 		if (screen_pos[2] > 0)
	// 		{
	// 			// ToDo: don't do this: scale by 100f & + half screen, do it better :D
	// 			screen_pos[0] = screen_pos[0] * 100.0f + app->main_window->width / 2;
	// 			screen_pos[1] = screen_pos[1] * 100.0f + app->main_window->height / 2;
	// 			// Make sure not outta buffer bounds
	// 			if (screen_pos[0] > 0 && screen_pos[0] < app->main_window->width &&
	// 				screen_pos[1] > 0 && screen_pos[1] < app->main_window->height)
	// 			{
	// 				app->main_window->frame_buf[
	// 					screen_to_frame_coords(app->main_window, screen_pos[0],
	// 					screen_pos[1])] = app->active_scene.objects[i].vertices[j].color;
	// 			}	
	// 		}
	// 	}
	// }
	return ;
}

void		update_frame(t_wolf3d *app)
{
	render_scene(app, app->active_scene);
}

void		render_background(t_wolf3d *app)
{
	int32_t		y;
	int32_t		x;
	uint32_t	color;

	color = 0x0;
	y = -1;
	while (++y < app->main_window->height)
	{
		x = -1;
		while (++x < app->main_window->width)
			app->main_window->framebuffer[
				screen_to_frame_coords(app->active_scene, x, y)] = color;
	}
}

void			draw_frame(t_wolf3d *app)
{
	(void)app;
	// SDL_LockTexture(app->main_window->frame, NULL,
	// 	(void**)&app->main_window->framebuffer,
	// 	&app->main_window->pitch);
	// render_background(app);

	update_frame(app);
	SDL_UpdateTexture(app->main_window->frame, NULL,
						app->main_window->framebuffer, WIDTH * sizeof(Uint32));
	// SDL_UnlockTexture(app->main_window->frame);
	SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->main_window->renderer);
}

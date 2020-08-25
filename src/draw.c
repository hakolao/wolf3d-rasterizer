/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 18:04:29 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Converts pixel position from screen coordinates to frame buffer index
** ToDo: Consider dynamic width
*/

static int		screen_to_frame_coords(t_window *window, int x, int y)
{
	return (y * window->width + x);
}

static void		draw_scene(t_wolf3d *app)
{
	size_t	i;
	size_t	j;
	t_vec4	screen_pos;

	i = -1;
	ft_memset(&screen_pos, 0, sizeof(t_vec4));
	while (++i < app->scene.object_count)
	{
		j = -1;
		while (++j < app->scene.objects[i].vertex_count)
		{
			camera_transform(&app->player.camera,
				app->scene.objects[i].vertices[j].position, screen_pos);
			// Check it's in front of camera
			if (screen_pos[2] > 0)
			{
				screen_pos[0] = screen_pos[0] * 100.0f + app->main_window->width / 2;
				screen_pos[1] = screen_pos[1] * 100.0f + app->main_window->height / 2;
				if (screen_pos[0] > 0 && screen_pos[0] < app->main_window->width &&
					screen_pos[1] > 0 && screen_pos[1] < app->main_window->height)
				{
					app->main_window->frame_buf[
						screen_to_frame_coords(app->main_window, screen_pos[0],
						screen_pos[1])] = app->scene.objects[i].vertices[j].color;
				}	
			}
		}
	}
}

static void		update_frame(t_wolf3d *app)
{
	draw_scene(app);
}

static void		render_background(t_wolf3d *app)
{
	int32_t		y;
	int32_t		x;
	uint32_t	color;

	color = 0xffffffff;
	y = -1;
	while (++y < app->main_window->height)
	{
		x = -1;
		while (++x < app->main_window->width)
			app->main_window->frame_buf[
				screen_to_frame_coords(app->main_window, x, y)] = color;
	}
}

void			draw_frame(t_wolf3d *app)
{
	SDL_LockTexture(app->main_window->frame, NULL,
		(void**)&app->main_window->frame_buf,
		&app->main_window->pitch);
	render_background(app);
	update_frame(app);
	SDL_UnlockTexture(app->main_window->frame);
	SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
		NULL, NULL);
	SDL_RenderPresent(app->main_window->renderer);
}

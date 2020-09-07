/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/07 20:08:56 by ohakola          ###   ########.fr       */
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

void		render_active_scene(t_wolf3d *app)
{
	(void)app;
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

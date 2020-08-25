/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:15:18 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 15:41:58 by ohakola          ###   ########.fr       */
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

static void		update_frame(t_wolf3d *app)
{
	(void)app;
	// ToDo: Apply transformation to vertices and frame buffer
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

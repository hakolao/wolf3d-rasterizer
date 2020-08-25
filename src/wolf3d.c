/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 14:15:11 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		wolf3d_main_loop(t_wolf3d *app)
{
	app->is_running = true;
	while (app->is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event))\
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_ESCAPE))
				app->is_running = false;
		}
		draw_frame(app);
	}
}

static void		wolf3d_init(t_wolf3d *app)
{
	init_player(app);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	SDL_DestroyRenderer(app->main_window->renderer);
	SDL_DestroyWindow(app->main_window->window);
	// ToDo: Free pixels
	free(app->main_window);
	IMG_Quit();
	SDL_Quit();
}

//test function, removable after use
void			modify_frame(unsigned int *pixels, int pitch)
{
	// for (int i = 0; i < WIDTH * HEIGHT / 2; i++)
	// {
	// 	pixels[i] = 0xffaaffff;
	// }
	pixels[screen_to_frame_coords(300, 300)] = 0xffaaffff;
	(void)pitch;
	return ;
}

void			render_frame(t_wolf3d *app) //testing only
{
	unsigned int	*pixels;
	int				pitch;

	pixels = (unsigned int *)malloc(sizeof(unsigned int) * WIDTH * HEIGHT);
	SDL_LockTexture(app->main_window->frame, NULL, (void **)&pixels, &pitch);
	modify_frame(pixels, pitch);
	SDL_UnlockTexture(app->main_window->frame);
	SDL_RenderCopy(app->main_window->renderer, app->main_window->frame,
				   NULL, NULL);
	SDL_RenderPresent(app->main_window->renderer);
}

void			wolf3d_run(t_wolf3d *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	main_window_init(app);
	wolf3d_init(app);

	render_frame(app);//testing only

	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}
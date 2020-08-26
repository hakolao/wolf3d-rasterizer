/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:08:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 18:03:55 by ohakola          ###   ########.fr       */
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
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_w)
					move_player(&app->player, forward);
				else if (event.key.keysym.sym == SDLK_s)
					move_player(&app->player, backward);
				else if (event.key.keysym.sym == SDLK_a)
					move_player(&app->player, strafe_left);
				else if (event.key.keysym.sym == SDLK_d)
					move_player(&app->player, strafe_right);
			}
		}
		draw_frame(app);
	}
}

static void		wolf3d_init(t_wolf3d *app)
{
	init_player(app);
	init_scene(app);
}

static void		wolf3d_cleanup(t_wolf3d *app)
{
	destroy_scene(app);
	SDL_DestroyRenderer(app->main_window->renderer);
	SDL_DestroyWindow(app->main_window->window);
	// ToDo: Free pixels
	free(app->main_window);
	IMG_Quit();
	SDL_Quit();
}

void			wolf3d_run(t_wolf3d *app)
{
	error_check(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
	main_window_init(app);
	wolf3d_init(app);
	// t_intersection is;
	t_vec3 dir = {0,0,1};
	t_vec3 origin = {1,1,-1};
	t_vec4 A = {0,0,0};
	t_vec4 B = {2,2,0};
	t_vec4 C = {4, 0, 0};
	t_ray ray;
	ray.dir[0] = dir[0];
	ray.dir[1] = dir[1];
	ray.dir[2] = dir[2];
	ray.origin[0] = origin[0];
	ray.origin[1] = origin[1];
	ray.origin[2] = origin[2];
	t_triangle triangle;
	triangle.a.position[0] = A[0];
	triangle.a.position[1] = A[1];
	triangle.a.position[2] = A[2];
	triangle.b.position[0] = B[0];
	triangle.b.position[1] = B[1];
	triangle.b.position[2] = B[2];
	triangle.c.position[0] = C[0];
	triangle.c.position[1] = C[1];
	triangle.c.position[2] = C[2];
	// intersect_triangle(triangle, ray, &is);
	placeholder();
		// dummyfunction();
	// ft_printf("%B\n", intersect_triangle(triangle, ray, &is));
	wolf3d_main_loop(app);
	wolf3d_cleanup(app);
}
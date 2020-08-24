/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:41:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include <SDL.h>
# include <SDL_image.h>
# include "libft.h"
# include "libgmatrix.h"
# include "lib3d.h"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define WIDTH 1280
# define HEIGHT 720
# define NAME "Wolf3D"

/*
**	Frame buffer
*/

# define BYTES_PER_PIXEL 8

typedef struct						s_window_info
{
	uint32_t				window_id;
	bool					is_hidden;
	void					*parent;
}									t_window_info;

typedef struct						s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	bool					resized;
	SDL_Window				*window;
	t_window_info			window_info;
}									t_window;

typedef struct						s_wolf3d
{
	bool					is_running;
	t_window				*main_window;
}									t_wolf3d;

void								wolf3d_run(t_wolf3d *app);

/*
** Draw / Render
*/
void								draw_frame(t_wolf3d *app);
int									screen_to_frame_coords(int x, int y);

/*
** Utils
*/
void								error_check(int test, const char *message);

/*
** Window
*/
void								main_window_init(t_wolf3d *app);

#endif
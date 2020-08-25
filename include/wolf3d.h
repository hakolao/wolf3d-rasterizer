/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 17:43:07 by ohakola          ###   ########.fr       */
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

typedef enum						e_move
{
	forward,
	backward,
	strafe_left,
	strafe_right
}									t_move;

typedef struct						s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	uint32_t				*frame_buf;
	int32_t					width;
	int32_t					height;
	int32_t					pitch;
	bool					resized;
	SDL_Window				*window;
	uint32_t				window_id;
	bool					is_hidden;
	void					*parent;
}									t_window;

typedef struct						s_camera
{
	t_mat4					model;
	t_mat4					view;
	t_mat4					projection;
}									t_camera;

typedef struct						s_player
{
	t_vec3					pos;
	t_vec3					forward;
	t_vec3					up;
	float					speed;
	float					rot_speed;
	t_camera				camera;
}									t_player;

//ToDo: Object to contain mesh(es) from lib3d

typedef struct						s_object
{
	t_vertex				*vertices;
	uint32_t				vertex_count;
}									t_object;

typedef struct						s_scene
{
	t_object				*objects;
	uint32_t				object_count;
}									t_scene;

typedef struct						s_wolf3d
{
	bool					is_running;
	t_window				*main_window;
	t_player				player;
	t_scene					scene;
}									t_wolf3d;

void								wolf3d_run(t_wolf3d *app);

/*
** Scene
*/
void								init_scene(t_wolf3d *app);
void								destroy_scene(t_wolf3d *app);

/*
** Player
*/
void								init_player(t_wolf3d *app);
void								move_player(t_player *player, t_move dir);

/*
** Camera
*/
void								update_camera_view(t_player *player);
void								init_camera(t_player *player);
void								camera_transform(t_camera *camera,
									t_vec4 vertex, t_vec4 res);

/*
** Draw / Render
*/
void								draw_frame(t_wolf3d *app);

/*
** Utils
*/
void								error_check(int test, const char *message);

/*
** Window
*/
void								main_window_init(t_wolf3d *app);

#endif
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

/*
**	The view scale will scale the camera and raycasting in relation to the
**	world and objects. So that bigger scale will effectively scale the "world"
**	smaller in the camera view without affecting the actual world.
*/

# define VIEW_SCALE 1
# define FPS 60
# define NAME "Wolf3D"

/*
**	The vector convention is a right handed coordinate system where up axis 
**	is z (2), left axis is y (1), forward axis is x (0). The numbers are for
**	easier referencing in t_vec3 structs.
*/

# define VEC_UP 2
# define VEC_LEFT 1
# define VEC_FORWARD 0

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

/*
**	Camera screen distance is the distance of the
**	virtual, screen through which the rays are projected,
**	from the camera in world units. The virtual screen dimensions
**	and the screen distance determine the field of view and vice versa.
*/

typedef struct						s_camera
{
	t_vec3					origin;
	t_vec3					orientation[3];
	Uint16					screen_width;
	Uint16					screen_height;
	float					fovx;
	float					fovy;
	float					screen_dist;

	t_mat4					view;
	t_mat4					model;
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

/*
**	Structs and typedefs used in rendering
*/

typedef void *(t_vertex_shader(char *fmt, ...));
typedef void *(t_fragment_shader(char *fmt, ...));

typedef struct						s_shader
{
	t_vertex_shader			*v;
	t_fragment_shader		*f;
}									t_shader;

typedef struct						s_mesh
{
	t_vertex				**vtc;
	int						vtx_count;
	t_triangle				*triangles;
	int						triangle_count;
	t_vec3					origin;
	t_vec3					orientation[3];
	t_box3d					bound_box;
	t_shader				shader;
	struct s_object			*parent_object;
}									t_mesh;

typedef struct						s_object
{
	t_mesh					*mesh;
	t_vec3					origin;
	t_vec3					orientation[3];
	t_vec3					parent_origin;
	t_vec3					parent_orientation[3];
	struct s_scene			*parent_scene;
}									t_object;

/*
**	Typedefs related to app and scene management
*/

typedef struct						s_scenedata
{
	int						level;
	//add here all the date needed to create a scene
	//for example fetch the map and included
	//objects. This will be passed to new_scene()
	//that will interpret the data and create the scene
}									t_scenedata;

typedef struct						s_scene
{
	t_object				**objects;
	uint32_t				object_count;
	t_camera				*main_camera;
}									t_scene;

typedef struct						s_wolf3d
{
	int						starting_tick;
	bool					is_running;
	t_window				*main_window;
	t_player				player;
	t_scene					*active_scene;
}									t_wolf3d;

/*
**	Function declarations
*/

void								wolf3d_run(t_wolf3d *app);
void								*init_app(t_wolf3d *app);
void								cap_framerate(Uint32 starting_tick);

/*
** Scene
*/

t_scene								*new_scene(t_wolf3d *app,
												t_scenedata *data);
void								destroy_scene(t_wolf3d *app);

/*
** Objects
*/

t_object							*create_object_triangle(t_scene *scene,
															t_wolf3d *app);

/*
** Player
*/

void								init_player(t_wolf3d *app);
void								move_player(t_player *player, t_move dir);

/*
** Camera
*/

t_camera							*new_camera(float screen_distance);
void								update_camera_view(t_player *player);
void								init_camera(t_player *player);
void								camera_transform(t_camera *camera,
									t_vec4 vertex, t_vec4 res);

/*
** Draw / Render
*/

void								draw_frame(t_wolf3d *app);
bool								render_mesh(t_mesh *mesh,
												t_camera *camera);

/*
** Utils
*/

void								error_check(int test, const char *message);

/*
** Window
*/

void								main_window_init(t_wolf3d *app);

#endif
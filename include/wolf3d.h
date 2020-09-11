/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/11 17:31:03 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "libft.h"
# include "libgmatrix.h"
# include "lib3d.h"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define FILE_READ_BUF 1024

# define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
# define GAME_FONT "assets/pixelated.ttf"
# define DEBUG_FONT "assets/Roboto-Regular.ttf"
# define FONT_SIZE 15

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

/*
** Forward declarations
*/

typedef struct						s_scene t_scene;
typedef struct						s_mesh t_mesh;

typedef enum						e_move
{
	move_forward,
	move_backward,
	move_strafe_left,
	move_strafe_right
}									t_move;

typedef enum						e_scene_id
{
	scene_id_main_menu,
	scene_id_main_game,
}									t_scene_id;

typedef enum						e_menu_actions
{
	menu_action_start_game,
	menu_action_load_game,
	menu_action_quit_game
}									t_menu_actions;

typedef struct						s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	uint32_t				*framebuffer;
	uint32_t				*zbuffer;
	TTF_Font				*font;
	int32_t					width;
	int32_t					height;
	int32_t					pitch;
	t_bool					resized;
	SDL_Window				*window;
	uint32_t				window_id;
	t_bool					is_hidden;
	void					*parent;
	uint32_t				framesize;
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
	uint32_t				screen_width;
	uint32_t				screen_height;
	float					fovx;
	float					fovy;
	float					screen_dist;
	t_ray					*rays;
	int						raycount;
	t_scene					*parent_scene;

	uint32_t				mallocsize;
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
}									t_player;

/*
**	Structs and typedefs used in rendering
*/

typedef struct						s_3d_object
{
	t_vec3					origin;
	t_vec3					position; // == origin * transform
	t_mat4					transform;
	t_vertex				*mesh_vertices;
	int32_t					mesh_vertex_count;
	t_triangle				*mesh_triangles;
	int32_t					mesh_triangle_count;
	t_mat4					mesh_transform;
	t_box3d					mesh_bound_box;
}									t_3d_object;

/*
**	Typedefs related to app and scene management
*/

typedef struct						s_scene_data
{
	int						level;
	t_scene_id				scene_id;
	const char				*menu_options[64];
	uint32_t				menu_option_count;
	t_camera				*main_camera;
	t_3d_object				**objects; //Read from map eventually
	int32_t					object_count; //Read from map eventually
}									t_scene_data;

struct s_scene
{
	t_3d_object				**objects;
	int32_t					object_count;
	t_camera				*main_camera;
	t_window				*main_window;
	const char				*menu_options[64];
	int32_t					menu_option_count;
	int32_t					selected_option;
	t_scene_id				scene_id;
};

typedef struct						s_wolf3d_debug
{
	uint32_t				fps;
	float					avg_delta_time;
}									t_wolf3d_debug;

typedef struct						s_wolf3d
{
	t_bool					is_running;
	t_bool					is_debug;
	t_wolf3d_debug			debug_info;
	t_window				*main_window;
	t_scene					*active_scene;
	uint32_t				time_since_start;
	uint32_t				delta_time;
	t_player				player;
}									t_wolf3d;

typedef struct						s_text_params
{
	const char				*font;
	const char				*text;
	SDL_Color				text_color;
	int						font_size;
	int						*xy;
	float					blend_ratio;
}									t_text_params;

typedef struct						s_file_contents
{
	void				*buf;
	uint32_t			size;
}									t_file_contents;

/*
**	Function declarations
*/

void								wolf3d_run(t_wolf3d *app);

/*
** Time
*/
void								cap_framerate(t_wolf3d *app);
float								sin_time(t_wolf3d *app,
									float min, float max, float speed);

/*
** Scene
*/

t_scene								*new_scene(t_wolf3d *app,
												t_scene_data *data);
void								destroy_scene(t_scene *scene);
void								set_active_scene(t_wolf3d *app,
									t_scene_id to_scene);
t_3d_object							**create_test_scene_objects(
									int32_t *object_count);

/*
**	3d Object
*/
t_3d_object							*create_3d_object(void);
void								init_3d_object(t_3d_object *obj,
									t_vertex *vertices,
									uint32_t vertex_count, t_vec3 origin);
void								init_triangle(t_triangle *triangle,
									t_vertex *pos_a, t_vertex *pos_b,
									t_vertex *pos_c);
void								destroy_object(t_3d_object *object);

/*
** Player
*/

void								init_player(t_wolf3d *app);
void								move_player(t_wolf3d *app, t_move dir);
void								rotate_player(t_wolf3d *app, t_vec3 axes);

/*
** Camera
*/

t_camera							*new_camera();
void								update_camera(t_wolf3d *app);
void								camera_transform(t_camera *camera,
									t_vec4 vertex, t_vec4 res);

/*
** Draw / Render
*/

void								draw_frame(t_wolf3d *app);
t_bool								render_mesh(t_wolf3d *app, t_mesh *mesh,
												t_camera *camera);
t_bool								render_triangle(t_wolf3d *app,
													t_triangle *triangle,
													t_mesh *mesh,
													t_camera *camera);
int									screen_to_frame_coords(uint32_t width,
															uint32_t height,
															int x, int y);
void								render_ui(t_wolf3d *app);

/*
** Utils
*/

void								error_check(int test, const char *message);

/*
** Window
*/

void								main_window_init(t_wolf3d *app);
void								recreate_frame(t_wolf3d *app);

/*
** Color utils
*/
uint32_t							rgba_to_u32(SDL_Color color);
uint32_t							color_blend_u32(uint32_t color1,
									uint32_t color2, float ratio);
SDL_Color							u32_to_rgba(uint32_t color);

/*
** Text
*/
void								render_text(t_wolf3d *app,
									t_text_params params);
void								render_blinking_text(t_wolf3d *app,
									t_text_params params);
void								render_centered_blinking_text(t_wolf3d *app,
									t_text_params params);
void								render_centered_text(t_wolf3d *app,
									t_text_params params);
uint32_t							get_relative_font_size(t_wolf3d *app,
									uint32_t font_size);

/*
** SDL Surface
*/
SDL_Surface							*surface_from_font(t_wolf3d *app,
									t_text_params params);
void								surface_to_framebuffer(t_wolf3d *app,
									SDL_Surface *surface, float blend_ratio,
									int xy[2]);

/*
** Debug information
*/
void								capture_framerate(t_wolf3d *app);
void								render_debug_grid(t_wolf3d *app);

/*
** File read
*/
void								read_obj(const char *filename);

#endif

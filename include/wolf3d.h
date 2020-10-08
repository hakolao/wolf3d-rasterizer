/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/08 15:08:58 by ohakola          ###   ########.fr       */
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
# include <float.h>

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
# define GAME_FONT "assets/pixelated.ttf"
# define DEBUG_FONT "assets/Roboto-Regular.ttf"
# define FONT_SIZE 20

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
# define SCREEN_INTERSECT_MAX FLT_MAX

/*
**	Player control macros
*/

# define PLAYER_SPEED 5
# define PLAYER_ROTATION_SPEED 0.1

/*
**	Frame buffer
*/

# define BYTES_PER_PIXEL 8

# define NEAR_CLIP_DIST 10
# define FAR_CLIP_DIST 100000

/*
** Forward declarations
*/

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
	uint32_t				*rbuffer;
	uint32_t				rbuf_render_color;
	TTF_Font				*main_font;
	TTF_Font				*debug_font;
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
	float					fov;
	float					screen_dist;
	int						width;
	int						height;
	float					near_clip;
	float					far_clip;
	t_plane					*viewplanes[6];
}									t_camera;

typedef struct						s_player
{
	t_vec3					pos;
	t_vec3					forward;
	t_vec3					sideways;
	t_vec3					up;
	float					speed;
	float					rot_speed;
}									t_player;

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
	t_3d_object				*objects[64];
	uint32_t				num_objects;
	uint32_t				num_triangles;
}									t_scene_data;

typedef struct						s_scene
{
	t_3d_object				*objects[64];
	uint32_t				num_objects;
	uint32_t				num_triangles;
	t_camera				*main_camera;
	t_triangle				*screen_triangles;
	const char				*menu_options[64];
	int32_t					menu_option_count;
	int32_t					selected_option;
	t_scene_id				scene_id;
	t_mat4					world_scale;
	t_mat4					world_rotation;
	t_mat4					world_translation;
}									t_scene;

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
	uint64_t				performance_start;
	uint64_t				performance_end;
	uint64_t				delta_time;
	t_player				player;
}									t_wolf3d;

typedef struct						s_text_params
{
	const char				*text;
	SDL_Color				text_color;
	TTF_Font 				*font;
	int						*xy;
	float					blend_ratio;
}									t_text_params;

typedef struct	s_ir
{
	int a;
	int b;
}				t_ir;

/*
**	Function declarations
*/

void						wolf3d_run(t_wolf3d *app);

/*
** Time
*/
void						cap_framerate(t_wolf3d *app);
float						sin_time(float min, float max, float speed);

/*
** Player
*/

void						init_player(t_wolf3d *app);
void						move_player(t_wolf3d *app, t_move dir);
void						rotate_player(t_wolf3d *app, t_vec3 axes);
void						player_action_handle(t_wolf3d *app, SDL_Event event);

/*
** Camera
*/

t_camera					*new_camera();
void						update_camera(t_wolf3d *app);
void						camera_transform(t_camera *camera,
							t_vec4 vertex, t_vec4 res);
void						create_screen_triangles(t_scene *scene);

/*
** Draw / Render
*/

void						draw_frame(t_wolf3d *app);
t_bool						render_triangle(t_wolf3d *app,
											t_triangle *triangle);
int							screen_to_frame_coords(uint32_t width,
													uint32_t height,
													int x, int y);
void						render_ui(t_wolf3d *app);

/*
** Utils
*/

void						error_check(int test, const char *message);

/*
** Window
*/

void						main_window_init(t_wolf3d *app);
void						recreate_frame(t_wolf3d *app);

/*
** Text
*/
void						render_text(t_wolf3d *app,
							t_text_params params);
void						render_blinking_text(t_wolf3d *app,
							t_text_params params);
void						render_centered_blinking_text(t_wolf3d *app,
							t_text_params params);
void						render_centered_text(t_wolf3d *app,
							t_text_params params);

/*
** Debug information
*/

uint64_t					capture_framerate(uint64_t delta_time);
void						render_debug_grid(t_wolf3d *app);

/*
** Scene
*/

void						scene_vertices_init(t_wolf3d *app,
							t_scene *scene);
t_scene						*new_scene(t_scene_data *data);
void						destroy_scene(t_scene *scene);
void						set_active_scene(t_wolf3d *app,
							t_scene_id to_scene);
t_3d_object					**create_scene1_objects(int32_t *obj_count);
void						debug_scene(t_scene *scene);
void						update_world_translation(t_scene *scene, t_mat4	new_translation);
void						update_world_rotation(t_scene *scene, t_mat4 new_rotation);
void						update_world_scale(t_scene *scene, t_mat4 new_scale);

#endif

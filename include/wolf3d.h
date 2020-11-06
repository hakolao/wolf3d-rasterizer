/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/06 13:54:32 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include "libgmatrix.h"
# include "lib3d.h"
# include <float.h>
# include "window.h"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define NUM_THREADS 4

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

# define PLAYER_SPEED 6.0
# define PLAYER_ROTATION_SPEED 0.2
# define PLAYER_HEIGHT WIDTH

# define NEAR_CLIP_DIST 10
# define FAR_CLIP_DIST 100000

typedef enum						e_render_pass
{
	rpass_rasterize,
	rpass_zbuffer
}									t_render_pass;

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
	t_plane					viewplanes[6];
	t_plane					screen;
}									t_camera;

typedef struct						s_player
{
	t_vec3					pos;
	t_vec3					forward;
	t_vec3					sideways;
	t_vec3					up;
	float					speed;
	float					rot_speed;
	float					rot_x;
	float					rot_y;
	t_mat4					rotation;
	t_mat4					inv_rotation;
	t_mat4					translation;
	t_mat4					inv_translation;
}									t_player;

/*
**	Typedefs related to app and scene management
*/

typedef struct						s_scene_data
{
	int						level;
	t_scene_id				scene_id;
	const char				*menu_options[128];
	uint32_t				menu_option_count;
	t_camera				*main_camera;
	t_3d_object				*objects[128];
	uint32_t				num_objects;
	uint32_t				num_triangles;
}									t_scene_data;

typedef struct						s_scene
{
	t_3d_object				*objects[128];
	uint32_t				num_objects;
	t_kd_tree				*bullet_tree;
	t_triangle				*triangle_ref[16384];
	uint32_t				num_triangles;
	t_camera				*main_camera;
	t_triangle				*screen_triangles;
	const char				*menu_options[128];
	int32_t					menu_option_count;
	int32_t					selected_option;
	t_scene_id				scene_id;
}									t_scene;

typedef struct						s_wolf3d
{
	t_bool					is_running;
	t_bool					is_debug;
	t_info					info;
	t_window				*window;
	t_scene					*active_scene;
	t_player				player;
	t_mouse					mouse;
	t_keyboard				keyboard;
	t_thread_pool			*thread_pool;
}									t_wolf3d;

/*
** For threading
*/
typedef struct						s_triangle_work
{
	t_wolf3d				*app;
	t_triangle				*triangle;
}									t_triangle_work;

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
void						rotate_player_vertical(t_wolf3d *app, float angle);
void						rotate_player_horizontal(t_wolf3d *app, float angle);

/*
** Events
*/
void						keyboard_state_set(t_wolf3d *app);
void						mouse_state_set(t_wolf3d *app);
void						mouse_state_handle(t_wolf3d *app);
void						keyboard_state_handle(t_wolf3d *app);
void						mouse_motion_handle(t_wolf3d *app, SDL_Event event);
void 						main_menu_event_handle(t_wolf3d *app, SDL_Event event);


/*
** Camera
*/
t_camera					*new_camera();
void						update_camera(t_wolf3d *app);

/*
** Render to framebuffer
*/
t_bool						screen_intersection(t_wolf3d *app,
								t_triangle *triangle);
t_bool						is_rendered(t_wolf3d *app, t_triangle *triangle);
void						ui_render(t_wolf3d *app);
void						wolf3d_render(t_wolf3d *app);

/*
** Scene
*/
void						scene_vertices_init(t_wolf3d *app,
								t_scene *scene);
t_scene						*new_scene(t_scene_data *data);
void						destroy_scene(t_scene *scene);
void						set_active_scene(t_wolf3d *app,
								t_scene_id to_scene);
void						debug_scene(t_scene *scene);

/*
** Debug
*/
void						wolf3d_debug_info_render(t_wolf3d *app);
void						wolf3d_debug_info_capture(t_wolf3d *app);

#endif

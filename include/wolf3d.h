/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/26 17:30:22 by ohakola+vei      ###   ########.fr       */
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

# define INITIAL_MAP_SIZE 15

# define NUM_THREADS 6

# define WIDTH 1280
# define HEIGHT 720
# define MAP_EDITOR_WIDTH 1280
# define MAP_EDITOR_HEIGHT 720
# define OPTIONAL_PATTERN 2

# define NAME "Wolf3D"
# define SCREEN_INTERSECT_MAX FLT_MAX

/*
**	Player control macros
*/

# define PLAYER_SPEED 6.0
# define PLAYER_ROTATION_SPEED 0.2

# define NEAR_CLIP_DIST 10
# define FAR_CLIP_DIST 100000

# define MAX_NUM_OBJECTS 16384
# define MAX_NUM_TRIANGLES 65536

# define NUM_ASSETS 64
# define TEMP_OBJECT_EXPIRE_SEC 100

typedef enum						e_render_pass
{
	rpass_rasterize = 1,
	rpass_zbuffer = 1 << 1
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
	scene_id_main_menu_settings,
	scene_id_main_game,
}									t_scene_id;

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
	t_vec2					grid_pos;
	t_bool					is_running;
	float					speed;
	float					rot_speed;
	float					rot_x;
	float					rot_y;
	t_mat4					rotation;
	t_mat4					inv_rotation;
	t_mat4					translation;
	t_mat4					inv_translation;
}									t_player;

typedef struct						s_wolf3d_map
{
	int32_t					size;
	float					render_size;
	t_vec2					render_pos;
	float					cell_render_size;
	t_hash_table			*map_images;
	uint32_t				*grid;
}									t_wolf3d_map;

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
	char					*map_filename;
	char					*texture_files[NUM_ASSETS];
	char					*model_files[NUM_ASSETS];
	uint32_t				asset_keys[NUM_ASSETS];
	uint32_t				num_assets_to_load;
}									t_scene_data;

typedef struct						s_scene
{
	t_wolf3d_map			*map;
	t_3d_object				*objects[MAX_NUM_OBJECTS];
	uint32_t				num_objects;
	t_kd_tree				*bullet_tree;
	t_triangle				*triangle_ref[MAX_NUM_TRIANGLES];
	uint32_t				num_triangles;
	t_camera				*main_camera;
	t_triangle				*screen_triangles;
	const char				*menu_options[128];
	int32_t					menu_option_count;
	int32_t					selected_option;
	t_bool					is_paused;
	t_scene_id				scene_id;
	char					*map_filename;
	t_hash_table			*textures;
	t_hash_table			*models;
	t_surface				*skybox_textures[6];
	t_3d_object				*skybox[6];
}									t_scene;

typedef struct						s_wolf3d
{
	t_bool					is_running;
	t_bool					is_debug;
	t_bool					is_loading;
	t_info					info;
	t_window				*window;
	t_scene_id				next_scene_id;
	t_scene					*active_scene;
	t_player				player;
	t_mouse					mouse;
	t_keyboard				keyboard;
	t_thread_pool			*thread_pool;
	float					unit_size;
	t_bool					is_minimap_largened;
}									t_wolf3d;

/*
** Map features used to draw the map in map editor
*/

typedef enum						e_map_features
{
	m_clear = 0,
	m_room = 1,
	m_start = 1 << 2,
	m_enemy = 1 << 7,
}									t_map_features;

/*
** Cell features saved into map & read from map file
*/

typedef enum						e_cell_features
{
	// Save first bit for whether cell is room or not. If not, nothing else
	// can exist there (see update_map_cell_features)
	c_floor = 1 << 1,
	c_floor_start = 1 << 2,
	c_wall_up = 1 << 3,
	c_wall_right = 1 << 4,
	c_wall_down = 1 << 5,
	c_wall_left = 1 << 6,
	c_block_nw = 1 << 7,
	c_block_ne = 1 << 8,
	c_block_se = 1 << 9,
	c_block_sw = 1 << 10,
}									t_cell_features;


/*
** Combination of cell features that form pieces based on neighbor
** cells
** corner block pieces are added separately (c_block_nw...)
*/

typedef enum						e_map_prefabs
{
	p_corr_vert = c_floor | c_wall_right | c_wall_left,
	p_corr_horz = c_floor | c_wall_up | c_wall_down,
	p_dead_up = c_floor | c_wall_left | c_wall_up | c_wall_right,
	p_dead_right = c_floor | c_wall_up | c_wall_right | c_wall_down,
	p_dead_down = c_floor | c_wall_right | c_wall_down | c_wall_left,
	p_dead_left = c_floor | c_wall_down | c_wall_left | c_wall_up,
	p_dead_all = c_floor | c_wall_up | c_wall_right | c_wall_down | c_wall_left,
	p_wall_up = c_floor | c_wall_up,
	p_wall_right = c_floor | c_wall_right,
	p_wall_down = c_floor | c_wall_down,
	p_wall_left = c_floor | c_wall_left,
	p_corner_se = c_floor | c_wall_down | c_wall_right,
	p_corner_sw = c_floor | c_wall_down | c_wall_left,
	p_corner_nw = c_floor | c_wall_up | c_wall_left,
	p_corner_ne = c_floor | c_wall_up | c_wall_right,
	p_middle_floor = c_floor,
	p_all = p_dead_all | c_block_nw | c_block_ne | c_block_se | c_block_sw
}									t_map_prefabs;

typedef struct						s_map_editor
{
	t_bool					is_running;
	t_info					info;
	t_window				*window;
	t_mouse					mouse;
	t_keyboard				keyboard;
	t_thread_pool			*thread_pool;
	t_button_group			*select_menu;
	t_button_group			*save_menu;
	t_wolf3d_map			*map;
	t_vec2					mouse_grid_pos;
	t_map_features			selected_feature;
	char					*filename;
}									t_map_editor;

/*
** For threading
*/
typedef struct						s_render_work
{
	t_wolf3d				*app;
	t_sub_framebuffer		*sub_buffer;
}									t_render_work;

void						wolf3d_run(t_wolf3d *app);

/*
** Time
*/
float						sin_time(float min, float max, float speed);

/*
** Player
*/
void						update_player_grid_pos(t_wolf3d *app);
void						init_player(t_wolf3d *app, t_vec3 pos);
void						move_player(t_wolf3d *app, t_move dir);
void						rotate_player_vertical(t_wolf3d *app, float angle);
void						rotate_player_horizontal(t_wolf3d *app, float angle);

/*
** Events
*/
void						handle_events(t_wolf3d *app);
void						keyboard_state_set(t_wolf3d *app);
void						mouse_state_set(t_wolf3d *app);
void						shooting_handle(t_wolf3d *app);
void						movement_handle(t_wolf3d *app);
void						mouse_motion_handle(t_wolf3d *app, SDL_Event event);
void 						main_menu_event_handle(t_wolf3d *app, SDL_Event event);
void 						main_menu_settings_event_handle(t_wolf3d *app, SDL_Event event);
void 						main_game_menu_event_handle(t_wolf3d *app, SDL_Event event);


/*
** Camera
*/
t_camera					*new_camera();
void						update_camera(t_wolf3d *app);

/*
** Rendering
*/
t_bool						screen_intersection(t_wolf3d *app,
								t_triangle *triangle);
t_bool						is_rendered(t_wolf3d *app, t_triangle *triangle);
void						ui_render(t_wolf3d *app);
void						wolf3d_render(t_wolf3d *app);
void						loading_render(t_wolf3d *app);
void						rasterize_skybox(t_render_work *work);
void						rasterize_objects(t_render_work *work);
void						render_triangle(t_wolf3d *app, t_sub_framebuffer *sub_buffer,
									t_triangle *triangle, t_render_pass passes);
void						set_render_triangle(t_wolf3d *app,
									t_triangle *r_triangle,
									t_triangle *triangle, t_vertex *vtc);
/*
** Scene
*/
void						scene_minimap_init(t_wolf3d *app);
void						scene_assets_load(t_scene *scene, t_scene_data *data);
void						scene_main_game_data_set(t_scene_data *data);
void						scene_main_menu_data_set(t_scene_data *data);
void						scene_settings_menu_data_set(t_scene_data *data);
t_scene						*scene_new(t_scene_data *data);
void						scene_destroy(t_scene *scene);
void						scene_next_select(t_wolf3d *app);
void						scene_debug(t_scene *scene);
void						scene_objects_generate(t_wolf3d *app,
								t_scene *scene);
void						scene_map_init(t_scene *scene);
void						scene_camera_destroy(t_scene *scene);
void						scene_objects_destroy(t_scene *scene);
void						scene_skybox_destroy(t_scene *scene);
void						scene_models_destroy(t_scene *scene);
void						scene_textures_destroy(t_scene *scene);

/*
** Debug
*/
void						wolf3d_debug_info_render(t_wolf3d *app);
void						wolf3d_debug_info_capture(t_wolf3d *app);

/*
** Map
*/

void						map_boundary_render(t_wolf3d_map *map,
								t_framebuffer *framebuffer,
								uint32_t grid_color);
void						map_minimap_render_full(t_wolf3d_map *map,
								t_framebuffer *framebuffer, t_player *player);
void						map_minimap_render_partial(t_wolf3d_map *map,
								t_framebuffer *framebuffer, float minimap_size,
								t_player *player);
void						map_features_render(t_wolf3d_map *map, t_framebuffer *framebuffer);
void						map_destroy(t_wolf3d_map *map);
void						map_init_image_assets(t_hash_table **map_images);
void						map_rescale_image_assets(t_wolf3d_map *map);
void						map_render_resize(t_wolf3d_map *map, float new_size,
								t_vec2 render_pos);

/*
** Map Editor
*/

void						map_editor_save_menu_create(t_map_editor *app);
void						map_editor_menu_render(t_map_editor *app, t_vec2 pos);
void						map_editor_draw_menu_create(t_map_editor *app);
void						map_editor_map_init(t_map_editor *app, int size);
void						map_editor_map_render(t_map_editor *app);
void						update_map_cell_features(t_map_editor *app);
t_bool						modify_surround_pattern(uint32_t *cell,
								int32_t rooms[9]);
t_bool						modify_wall_pattern(uint32_t *cell,
								int32_t rooms[9]);
t_bool						modify_corridor_pattern(uint32_t *cell,
								int32_t rooms[9]);
t_bool						modify_corner_pattern(uint32_t *cell,
								int32_t rooms[9]);
t_bool						modify_dead_end_pattern(uint32_t *cell,
								int32_t rooms[9]);

#endif

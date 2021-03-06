/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/09 19:45:05 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Wolf3d ray cast software renderer project has been written
** by Okko Hakola & Vesa Eilo
*/

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
# define NUM_THREADS_DEFAULT 4
# define WIDTH 1280
# define HEIGHT 720
# define MAP_EDITOR_WIDTH 1280
# define MAP_EDITOR_HEIGHT 720
# define OPTIONAL_PATTERN 2
# define NAME "Wolf3D"
# define SCREEN_INTERSECT_MAX FLT_MAX
# define PLAYER_SPEED 6.0
# define PLAYER_ROTATION_SPEED 0.2
# define NEAR_CLIP_DIST 10
# define FAR_CLIP_DIST 100000
# define MAX_NUM_OBJECTS 16384
# define NUM_ASSETS 64
# define TEMP_OBJECT_EXPIRE_SEC 100

typedef enum				e_move
{
	move_forward,
	move_backward,
	move_strafe_left,
	move_strafe_right,
	move_down,
	move_up,
}							t_move;

typedef enum				e_scene_id
{
	scene_id_main_menu,
	scene_id_main_menu_settings,
	scene_id_main_game,
}							t_scene_id;

typedef struct				s_camera
{
	t_vec3					origin;
	float					near_clip;
	float					far_clip;
	t_plane					viewplanes[6];
	t_plane					screen;
}							t_camera;

typedef struct				s_player
{
	t_vec3					pos;
	t_vec3					forward;
	t_vec3					sideways;
	t_vec3					up;
	t_vec2					grid_pos;
	t_bool					is_running;
	t_bool					is_shooting;
	t_bool					is_moving;
	t_bool					is_rotating;
	float					speed;
	float					rot_speed;
	float					rot_x;
	float					rot_y;
	float					player_height;
	float					fire_rate_per_sec;
	t_mat4					rotation;
	t_mat4					inv_rotation;
	t_mat4					translation;
	t_mat4					inv_translation;
	t_box3d					aabb;
}							t_player;

typedef struct				s_wolf3d_map
{
	int32_t					size;
	float					render_size;
	t_vec2					render_pos;
	float					cell_render_size;
	t_hash_table			*map_images;
	uint32_t				*grid;
}							t_wolf3d_map;

typedef struct				s_scene_data
{
	int						level;
	t_scene_id				scene_id;
	const char				*menu_options[128];
	uint32_t				menu_option_count;
	t_camera				*main_camera;
	char					*map_filename;
	char					*texture_files[NUM_ASSETS];
	char					*normal_map_files[NUM_ASSETS];
	char					*model_files[NUM_ASSETS];
	uint32_t				asset_keys[NUM_ASSETS];
	uint32_t				num_assets_to_load;
}							t_scene_data;

typedef struct				s_scene
{
	t_wolf3d_map			*map;
	t_3d_object				*objects[MAX_NUM_OBJECTS];
	uint32_t				num_objects;
	t_kd_tree				*triangle_tree;
	t_triangle				**triangle_ref;
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
	t_hash_table			*normal_maps;
	t_hash_table			*models;
	t_surface				*skybox_textures[6];
	t_3d_object				*skybox[6];
}							t_scene;

typedef struct				s_wolf3d
{
	t_bool					is_running;
	t_bool					is_debug;
	t_bool					is_loading;
	t_bool					is_normal_map;
	t_bool					is_first_render;
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
	int32_t					triangles_in_view;
}							t_wolf3d;

/*
** Map features used to draw the map in map editor
*/

typedef enum				e_map_features
{
	m_clear = 0,
	m_room = 1,
	m_start = 1 << 2,
	m_enemy = 1 << 7,
}							t_map_features;

/*
** Cell features saved into map & read from map file
** First bit determines if one is a room or not.
*/

typedef enum				e_cell_features
{
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
	c_corner_nw = 1 << 11,
	c_corner_ne = 1 << 12,
	c_corner_se = 1 << 13,
	c_corner_sw = 1 << 14,
	c_ceiling = 1 << 15,
	c_ceiling_window = 1 << 16,
}							t_cell_features;

/*
** Combination of cell features that form pieces based on neighbor
** cells
** corner block pieces are added separately (c_block_nw...)
*/

typedef enum				e_map_prefabs
{
	p_corr_vert = c_floor | c_wall_right | c_wall_left,
	p_corr_horz = c_floor | c_wall_up | c_wall_down,
	p_dead_up = c_floor | c_wall_left | c_wall_up | c_wall_right |
	c_corner_ne | c_corner_nw,
	p_dead_right = c_floor | c_wall_up | c_wall_right | c_wall_down |
	c_corner_se | c_corner_ne,
	p_dead_down = c_floor | c_wall_right | c_wall_down | c_wall_left |
	c_corner_se | c_corner_sw,
	p_dead_left = c_floor | c_wall_down | c_wall_left | c_wall_up |
	c_corner_sw | c_corner_nw,
	p_dead_all = c_floor | c_wall_up | c_wall_right | c_wall_down |
	c_wall_left | c_corner_se | c_corner_ne | c_corner_sw | c_corner_nw,
	p_wall_up = c_floor | c_wall_up,
	p_wall_right = c_floor | c_wall_right,
	p_wall_down = c_floor | c_wall_down,
	p_wall_left = c_floor | c_wall_left,
	p_corner_se = c_floor | c_wall_down | c_wall_right | c_corner_se,
	p_corner_sw = c_floor | c_wall_down | c_wall_left | c_corner_sw,
	p_corner_nw = c_floor | c_wall_up | c_wall_left | c_corner_nw,
	p_corner_ne = c_floor | c_wall_up | c_wall_right | c_corner_ne,
	p_middle_floor = c_floor,
	p_ceiling = c_floor | c_ceiling,
	p_ceiling_window = c_floor | c_ceiling_window,
	p_all = p_dead_all | c_block_nw | c_block_ne | c_block_se |
	c_block_sw | p_ceiling,
}							t_map_prefabs;

typedef struct				s_map_editor
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
}							t_map_editor;

/*
** For parallelization
*/

typedef struct				s_render_work
{
	t_wolf3d				*app;
	t_sub_framebuffer		*sub_buffer;
	t_tri_vec				*render_triangles;
}							t_render_work;

void						wolf3d_run(t_wolf3d *app);

/*
** Player
*/
void						pos_to_grid_pos(t_vec3 pos, t_vec2 grid_pos,
								float unit_size);
void						player_init(t_wolf3d *app, t_vec3 pos);
void						player_move(t_wolf3d *app, t_move dir, float speed);
void						player_rotate_vertical(t_wolf3d *app, float angle);
void						player_rotate_horizontal(t_wolf3d *app,
								float angle);
void						player_apply_gravity(t_wolf3d *app);
void						collision_limit_player(t_wolf3d *app, t_vec3 add);
void						player_update_aabb(t_player *player);
void						position_limit_player(t_wolf3d *app, t_vec3 add);
void						place_player(t_wolf3d *app, float unit_size,
								int32_t xy_rot[3]);

/*
** Events
*/
void						events_handle(t_wolf3d *app);
void						mouse_state_handle(t_wolf3d *app);
void						player_shoot(t_wolf3d *app,
								uint32_t curr_time);
void						keyboard_state_handle(t_wolf3d *app);
void						main_menu_event_handle(t_wolf3d *app,
								SDL_Event event);
void						main_menu_settings_event_handle(t_wolf3d *app,
								SDL_Event event);
void						main_game_menu_event_handle(t_wolf3d *app,
								SDL_Event event);
void						general_input_events_handle(t_wolf3d *app,
								SDL_Event event);

/*
** Camera
*/
t_camera					*new_camera(void);
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
t_tri_vec					*prepare_render_triangles(t_wolf3d *app);
void						destroy_render_triangles(
								t_tri_vec *render_triangles);
void						clip_and_add_to_render_triangles(t_wolf3d *app,
								t_tri_vec *r_triangle_vec,
								t_triangle *triangle);
void						rasterize_triangles(t_render_work *work);
t_bool						triangle_inside_viewbox(t_wolf3d *app,
								t_triangle *triangle);
t_bool						triangle_too_far(t_wolf3d *app,
								t_triangle *triangle);
void						prepare_skybox_render_triangle(t_wolf3d *app,
								t_triangle *r_triangle,
								t_triangle *triangle, t_vertex *vtc);
void						prepare_render_triangle(t_wolf3d *app,
								t_triangle *r_triangle,
								t_triangle *triangle, t_vertex *vtc);
t_bool						object_inside_viewbox(t_wolf3d *app,
								t_3d_object *obj);
void						ui_main_game_render(t_wolf3d *app);
void						framebuffer_dark_overlay(
								t_framebuffer *framebuffer,
								int32_t width, int32_t height, t_vec2 pos);
void						set_aabb_origin_to_corners(t_3d_object *obj,
								t_vec3 origin, t_vec3 origin_to_corner[8]);

/*
** Scene
*/
void						main_scene_data_asset_files_set(t_scene_data *data);
void						scene_minimap_init(t_wolf3d *app);
void						scene_assets_load(t_scene *scene,
								t_scene_data *data);
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
void						scene_normal_maps_destroy(t_scene *scene);

/*
** Debug
*/
void						wolf3d_debug_info_render(t_wolf3d *app);
void						wolf3d_debug_info_capture(t_wolf3d *app);
uint64_t					wolf3d_performance_counter_start(void);
void						wolf3d_performance_counter_end(uint64_t start_time,
								char *task_name, float delta_limit);

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
void						map_features_render(t_wolf3d_map *map,
								t_framebuffer *framebuffer);
void						map_destroy(t_wolf3d_map *map);
void						map_init_image_assets(t_hash_table **map_images);
void						map_rescale_image_assets(t_wolf3d_map *map);
void						map_render_resize(t_wolf3d_map *map, float new_size,
								t_vec2 render_pos);
void						minimap_size_update(t_wolf3d *app);
void						player_dir_render(t_framebuffer *framebuffer,
								t_player *player, t_vec2 player_render_pos,
								float player_render_size);

/*
** Map Editor
*/

t_surface					*convert_sdl_surface_to_t_surface(SDL_Surface *src);
t_bool						rooms_eq_pattern(int32_t rooms[9],
								int32_t pattern[9]);
void						update_mouse_grid_pos(t_map_editor *app);
void						handle_feature_placement(t_map_editor *app);
void						map_editor_save_menu_create(t_map_editor *app);
void						map_editor_menu_render(t_map_editor *app,
								t_vec2 pos);
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

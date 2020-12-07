/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 02:45:54 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include "libft.h"
# include "lib3d.h"
# ifdef __APPLE__
#  include <SDL.h>
#  include <SDL_image.h>
#  include <SDL_ttf.h>
# endif
# ifdef __linux__
#  include <SDL2/SDL.h>
#  include <SDL2/SDL_image.h>
#  include <SDL2/SDL_ttf.h>
# endif

# define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888
# define GAME_FONT "assets/fonts/wolfenstein.ttf"
# define DEBUG_FONT "assets/fonts/Roboto-Regular.ttf"
# define FONT_SIZE 50

/*
**	Frame buffer
*/

# define BYTES_PER_PIXEL 8

typedef struct				s_text_params
{
	const char				*text;
	SDL_Color				text_color;
	int						*xy;
	float					blend_ratio;
}							t_text_params;

typedef struct				s_info
{
	uint32_t				fps;
	uint64_t				delta_time;
	uint64_t				performance_start;
	uint64_t				performance_end;
}							t_info;

typedef struct				s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	t_framebuffer			*framebuffer;
	TTF_Font				*main_font;
	TTF_Font				*debug_font;
	TTF_Font				*title_font;
	int32_t					width;
	int32_t					height;
	int32_t					pitch;
	t_bool					resized;
	SDL_Window				*window;
	uint32_t				window_id;
	t_bool					is_hidden;
	t_bool					is_fullscreen;
}							t_window;

typedef struct				s_mouse
{
	int32_t				x;
	int32_t				y;
	uint32_t			state;
}							t_mouse;

typedef struct				s_keyboard
{
	const uint8_t		*state;
}							t_keyboard;

typedef struct s_button		t_button;

struct						s_button
{
	uint32_t				id;
	t_vec2					pos;
	uint32_t				width;
	uint32_t				height;
	t_surface				*texture;
	t_surface				*texture_down;
	t_bool					is_down;
	t_bool					is_hovered;
	void					(*on_click)(t_button *, void *);
	void					(*on_hover)(t_button *, void *);
	void					*on_click_params;
	void					*on_hover_params;
	t_window				*window;
};

typedef struct				s_button_group
{
	t_vec2					pos;
	t_button				**buttons;
	uint32_t				num_buttons;
	uint32_t				space_between;
	t_bool					is_horizontal;
	t_bool					is_selector;
	uint32_t				selected_index;
}							t_button_group;

/*
** Window
*/

void						window_destroy(t_window *window);
void						window_create(t_window **window_ref,
								int32_t width, int32_t height);
void						window_frame_recreate(t_window *window);
void						window_frame_draw(t_window *window);
void						window_frame_clear(t_window *window);
void						window_resize(t_window *window,
								int32_t width, int32_t height);

/*
** Text
*/

void						window_text_render(t_window *window,
								t_text_params params, TTF_Font *font);
void						window_text_render_centered(t_window *window,
								t_text_params params, TTF_Font *font);
SDL_Surface					*surface_from_font(t_window *window,
								t_text_params params,
								TTF_Font *font);
/*
** Utils
*/

void						window_fps_draw(t_window *window,
								uint32_t fps, uint64_t delta_time);
uint32_t					window_framerate_capture(uint32_t time_since_start);

/*
** Buttons
*/
void						button_group_destroy(t_button_group *button_group);
void						button_group_update_position(
								t_button_group *button_group,
								t_vec2 pos);
void						button_group_set_horizontal(
								t_button_group *button_group);
t_button_group				*button_group_create(t_button **buttons,
								uint32_t num_buttons);
void						button_destroy(t_button *button);
void						button_update_position(t_button *button,
								t_vec2 pos);
void						button_set_handles(t_button *button,
								void (*on_click)(t_button *, void *),
								void (*on_hover)(t_button *, void *));
void						button_set_texture(t_button *button,
								t_surface *texture,
								t_surface *texture_down);
t_button					*button_create(t_window *window, uint32_t id);
void						button_set_handle_params(t_button *button,
								void *on_click_params,
								void *on_hover_params);
void						button_render(t_button *button);
void						button_group_render(t_button_group *button_group);
void						button_group_set_space_between(
								t_button_group *button_group,
								int32_t space_between);
void						button_group_events_handle(
								t_button_group *button_group,
								t_mouse mouse, SDL_Event event);
void						button_events_handle(t_button *button,
								t_mouse mouse, SDL_Event event);
void						button_group_set_selector(
								t_button_group *button_group,
								int32_t	selected_index);
t_bool						button_is_clicked(t_button *button, t_mouse mouse,
								SDL_Event event);
t_bool						button_is_down(t_button *button, t_mouse mouse,
								SDL_Event event);
t_bool						button_is_hovered(t_button *button, t_mouse mouse,
								SDL_Event event);
void						button_state_handle(t_button *button, t_mouse mouse,
								SDL_Event event);

#endif

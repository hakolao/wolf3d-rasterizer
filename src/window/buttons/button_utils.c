/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 11:57:08 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/11 16:59:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void			button_set_texture(t_button *button, t_surface *texture,
					t_surface *texture_down)
{
	if (texture && texture_down)
		error_check(!(texture->w == texture_down->w &&
			texture->h == texture_down->h),
			"Texture and texture down width and height not equal");
	error_check(!texture, "No texture given, invalid button use");
	button->texture = texture;
	button->texture_down = texture_down;
	button->width = texture->w;
	button->height = texture->h;
}

void			button_set_handles(t_button *button,
					void (*on_click)(t_button *, void *),
					void (*on_hover)(t_button *, void *))
{
	button->on_click = on_click;
	button->on_hover = on_hover;
}

void			button_set_handle_params(t_button *button,
					void *on_click_params,
					void *on_hover_params)
{
	button->on_click_params = on_click_params;
	button->on_hover_params = on_hover_params;
}

void			button_update_position(t_button *button, t_vec2 pos)
{
	ml_vector2_copy(pos, button->pos);
}

void			button_render(t_button *button)
{
	if (!button->texture)
		return ;
	if (button->is_down)
		l3d_framebuffer_image_place(
			&(t_surface){
				.h = button->window->height,
				.w = button->window->width,
				.pixels = button->window->buffers->framebuffer},
			button->texture_down ? button->texture_down : button->texture, 
			(int32_t[2]){button->pos[0], button->pos[1]},
				button->is_hovered ? 0.5 : 1.0);
	else
		l3d_framebuffer_image_place(
			&(t_surface){
				.h = button->window->height,
				.w = button->window->width,
				.pixels = button->window->buffers->framebuffer},
			button->texture, 
			(int32_t[2]){button->pos[0], button->pos[1]},
			button->is_hovered ? 0.5 : 1.0);	
}

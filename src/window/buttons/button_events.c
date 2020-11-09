/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 11:58:12 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/09 19:26:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

t_bool			button_is_clicked(t_button *button, t_mouse mouse,
					SDL_Event event)
{
	t_bool		res;

	res = false;
	if (mouse.x >= button->pos[0] &&
		mouse.x < button->pos[0] + button->width &&
		mouse.y >= button->pos[1] &&
		mouse.y < button->pos[1] + button->height)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			button->on_click(button->on_click_params);
			res = true;
			button->is_down = true;
			button->is_hovered = false;
		}
		else if (!button->is_down)
		{
			button->on_hover(button->on_hover_params);
			button->is_hovered = true;
		}
	}
	else
	{
		button->is_hovered = false;
		button->is_down = false;
	}
	return (res);
}

void			button_group_events_handle(t_button_group *button_group,
					t_mouse mouse, SDL_Event event)
{
	int32_t		i;

	i = -1;
	if (!button_group->is_selector)
	{
		while (++i < (int32_t)button_group->num_buttons)
			button_is_clicked(button_group->buttons[i], mouse, event);
	}
	else
	{
		while (++i < (int32_t)button_group->num_buttons)
		{
			if (button_is_clicked(button_group->buttons[i], mouse, event))
				button_group->selected_index = i;
			if (i == (int32_t)button_group->selected_index)
				button_group->buttons[i]->is_down = true;
			else
				button_group->buttons[i]->is_down = false;
		}
	}
}

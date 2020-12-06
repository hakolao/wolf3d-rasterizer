/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 23:22:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 23:24:50 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

t_bool			button_is_down(t_button *button, t_mouse mouse,
					SDL_Event event)
{
	t_bool		res;

	res = false;
	if (mouse.x >= button->pos[0] &&
		mouse.x < button->pos[0] + button->width &&
		mouse.y >= button->pos[1] &&
		mouse.y < button->pos[1] + button->height)
	{
		if ((event.type == SDL_MOUSEBUTTONDOWN &&
			event.button.button == SDL_BUTTON_LMASK) ||
			(mouse.state & SDL_BUTTON_LMASK))
			return (true);
	}
	return (res);
}

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
		if (event.type == SDL_MOUSEBUTTONUP &&
			event.button.button == SDL_BUTTON_LMASK)
			return (true);
	}
	return (res);
}

t_bool			button_is_hovered(t_button *button, t_mouse mouse,
					SDL_Event event)
{
	t_bool		res;

	res = false;
	if (mouse.x >= button->pos[0] &&
		mouse.x < button->pos[0] + button->width &&
		mouse.y >= button->pos[1] &&
		mouse.y < button->pos[1] + button->height)
	{
		if (!button_is_clicked(button, mouse, event) &&
			!button_is_down(button, mouse, event))
			return (true);
	}
	return (res);
}

void			button_state_handle(t_button *button, t_mouse mouse,
					SDL_Event event)
{
	if (button_is_clicked(button, mouse, event))
	{
		if (button->on_click != NULL)
			button->on_click(button, button->on_click_params);
		button->is_down = false;
		button->is_hovered = true;
	}
	else if (button_is_hovered(button, mouse, event))
	{
		if (button->on_hover != NULL)
			button->on_hover(button, button->on_hover_params);
		button->is_hovered = true;
		button->is_down = false;
	}
	else if (button_is_down(button, mouse, event))
	{
		button->is_down = true;
		button->is_hovered = false;
	}
	else
	{
		button->is_down = false;
		button->is_hovered = false;
	}

}

static void		button_selector_state_handle(t_button_group *button_group,
					t_mouse mouse, SDL_Event event, int32_t i)
{
	if (i == (int32_t)button_group->selected_index)
	{
		button_group->buttons[i]->is_down = true;
		button_group->buttons[i]->is_hovered = false;
	}
	else
		button_group->buttons[i]->is_down = false;
	if (button_is_down(button_group->buttons[i], mouse, event))
	{
		button_group->buttons[i]->is_down = true;
		button_group->buttons[i]->is_hovered = false;
	}
	else if (button_is_hovered(button_group->buttons[i], mouse, event)
		&& !button_group->buttons[i]->is_down)
	{
		button_group->buttons[i]->is_hovered = true;
		button_group->buttons[i]->is_down = false;
	}
	else
		button_group->buttons[i]->is_hovered = false;
}

void			button_group_events_handle(t_button_group *button_group,
					t_mouse mouse, SDL_Event event)
{
	int32_t		i;

	i = -1;
	if (!button_group->is_selector)
	{
		while (++i < (int32_t)button_group->num_buttons)
			button_state_handle(button_group->buttons[i], mouse, event);
	}
	else
	{
		while (++i < (int32_t)button_group->num_buttons)
		{
			if (button_is_clicked(button_group->buttons[i], mouse, event))
			{
				button_group->selected_index = i;
				if (button_group->buttons[i]->on_click != NULL)
					button_group->buttons[i]->on_click(button_group->buttons[i],
					button_group->buttons[i]->on_click_params);
			}
			button_selector_state_handle(button_group, mouse, event, i);
		}
	}
}

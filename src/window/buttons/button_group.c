/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_group.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 11:55:04 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/20 12:41:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

t_button_group	*button_group_create(t_button **buttons, uint32_t num_buttons)
{
	t_button_group	*button_group;
	uint32_t		max_width;
	uint32_t		max_height;
	int32_t			i;

	if (buttons == NULL || num_buttons == 0)
		return (NULL);
	error_check(!(button_group = malloc(sizeof(t_button_group))),
		"Failed to malloc btn group");
	ft_memset(button_group, 0, sizeof(t_button_group));
	button_group->num_buttons = num_buttons;
	button_group->buttons = buttons;
	button_group->is_horizontal = false;
	i = -1;
	max_width = buttons[0]->width;
	max_height = buttons[0]->height;
	while (++i < (int32_t)num_buttons)
	{
		if (buttons[i]->width > max_width)
			max_width = buttons[i]->width;
		if (buttons[i]->height > max_height)
			max_height = buttons[i]->height;
	}
	while (--i >= 0)
	{
		buttons[i]->width = max_width;
		buttons[i]->height = max_height;
	}
	button_group_update_position(button_group, button_group->pos);
	return (button_group);
}



void			button_group_destroy(t_button_group *button_group)
{
	int32_t		i;

	i = -1;
	while (++i < (int32_t)button_group->num_buttons)
		button_destroy(button_group->buttons[i]);
	free(button_group->buttons);
	free(button_group);
	button_group = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_group_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 11:58:48 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/20 12:42:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void			button_group_set_horizontal(t_button_group *button_group)
{
	button_group->is_horizontal = true;
	button_group_update_position(button_group, button_group->pos);
}

void			button_group_set_space_between(t_button_group *button_group,
					int32_t space_between)
{
	button_group->space_between = space_between;
	button_group_update_position(button_group, button_group->pos);
}

void			button_group_set_selector(t_button_group *button_group,
					int32_t	selected_index)
{
	button_group->is_selector = true;
	button_group->selected_index = selected_index;
}

void			button_group_update_position(t_button_group *button_group,
					t_vec2 pos)
{
	int32_t		i;
	t_vec2		button_pos;
	int32_t		width_sum;
	int32_t		height_sum;
	
	ml_vector2_copy(pos, button_group->pos);
	i = -1;
	width_sum = 0;
	height_sum = 0;
	while (++i < (int32_t)button_group->num_buttons)
	{
		if (button_group->is_horizontal)
		{
			width_sum +=button_group->buttons[i]->width +
				button_group->space_between;
			ml_vector2_add(button_group->pos,
				(t_vec2){button_group->pos[0] + width_sum,
				button_group->pos[1]}, button_pos);
		}
		else
		{
			height_sum +=button_group->buttons[i]->height +
				button_group->space_between;
			ml_vector2_add(button_group->pos, (t_vec2){button_group->pos[0],
				button_group->pos[1] + height_sum}, button_pos);
		}
		button_update_position(button_group->buttons[i], button_pos);
	}
}

void			button_group_render(t_button_group *button_group)
{
	int32_t		i;

	i = -1;
	while (++i < (int32_t)button_group->num_buttons)
		button_render(button_group->buttons[i]);
}

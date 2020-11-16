/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 17:08:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:56:20 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

t_button		*button_create(t_window *window, uint32_t id)
{
	t_button		*button;

	error_check(!(button = malloc(sizeof(t_button))), "Failed to malloc btn");
	ft_memset(button, 0, sizeof(t_button));
	button->id = id;
	button->window = window;
	return (button);
}

void			button_destroy(t_button *button)
{
	if (button->texture)
		free(button->texture->pixels);
	if (button->texture_down)
		free(button->texture_down->pixels);
	free(button);
	button = NULL;
}

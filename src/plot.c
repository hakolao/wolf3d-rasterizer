/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plot.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:31:24 by veilo             #+#    #+#             */
/*   Updated: 2020/09/14 17:31:24 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	plot_pixel(t_wolf3d *app, uint32_t *buffer, int *point, uint32_t color)
{
	buffer[screen_to_frame_coords(
		app->main_window->width, app->main_window->height, point[0], point[1])] =
		color;
}
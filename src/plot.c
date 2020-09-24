/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plot.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:31:24 by veilo             #+#    #+#             */
/*   Updated: 2020/09/24 16:58:44 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	plot_pixel(t_wolf3d *app, uint32_t *buffer, int point[2], uint32_t color)
{
	if (point[0] < 0 || point[0] >= app->main_window->width ||
		point[1] < 0 || point[1] >= app->main_window->height)
		return ;
	buffer[screen_to_frame_coords(
		app->main_window->width, app->main_window->height, point[0],
		point[1])] = color;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_drawing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:51:22 by veilo             #+#    #+#             */
/*   Updated: 2020/09/14 17:51:23 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void plot_line_low(int *p1, int *p2, uint32_t color, t_wolf3d *app)
{
	int dxy[2];
	int dyi[2];
	int xy[2];

	dxy[0] = p2[0] - p1[0];
	dxy[1] = p2[1] - p1[1];
	dyi[1] = 1;
	if (dxy[1] < 0)
	{
		dyi[1] = -1;
		dxy[1] = -dxy[1];
	}
	dyi[0] = 2 * dxy[1] - dxy[0];
	xy[1] = p1[1];
	xy[0] = p1[0] - 1;
	while (++xy[0] <= p2[0])
	{
		plot_pixel(app, app->main_window->rbuffer, xy, color);
		if (dyi[0] > 0)
		{
			xy[1] += dyi[1];
			dyi[0] -= 2 * dxy[0];
		}
		dyi[0] += 2 * dxy[1];
	}
}

static void plot_line_high(int *p1, int *p2, uint32_t color, t_wolf3d *app)
{
	int dxy[2];
	int dxi[2];
	int xy[2];

	dxy[0] = p2[0] - p1[0];
	dxy[1] = p2[1] - p1[1];
	dxi[1] = 1;
	if (dxy[0] < 0)
	{
		dxi[1] = -1;
		dxy[0] = -dxy[0];
	}
	dxi[0] = 2 * dxy[0] - dxy[1];
	xy[0] = p1[0];
	xy[1] = p1[1] - 1;
	while (++xy[1] <= p2[1])
	{
		plot_pixel(app, app->main_window->rbuffer, xy, color);
		if (dxi[0] > 0)
		{
			xy[0] += dxi[1];
			dxi[0] -= 2 * dxy[1];
		}
		dxi[0] += 2 * dxy[0];
	}
}

static void plot_vertical(int *begin, int *finish, uint32_t color, t_wolf3d *app)
{
	if (begin[1] <= finish[1])
	{
		begin[1]--;
		while (begin[1] <= finish[1])
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){begin[0], begin[1]}, color);
			begin[1]++;
		}
	}
	else if (begin[1] >= finish[1])
	{
		finish[1]--;
		while (begin[1] >= finish[1])
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){finish[0], finish[1]}, color);
			finish[1]++;
		}
	}
}

static void plot_horizontal(int *begin, int *finish, uint32_t color, t_wolf3d *app)
{
	if (begin[0] <= finish[0])
	{
		begin[0]--;
		while (begin[0] <= finish[0])
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){begin[0], begin[1]}, color);
			begin[0]++;
		}
	}
	else if (begin[0] >= finish[0])
	{
		finish[0]--;
		while (begin[0] >= finish[0])
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){finish[0], finish[1]}, color);
			finish[0]++;
		}
	}
}

void draw_line(int *begin, int *finish, uint32_t color, t_wolf3d *app)
{
	if (begin[0] == finish[0])
		plot_vertical(begin, finish, color, app);
	else if (begin[1] == finish[1])
		plot_horizontal(begin, finish, color, app);
	else
	{
		if (ft_abs(finish[1] - begin[1]) < ft_abs(finish[0] - begin[0]))
		{
			if (begin[0] > finish[0])
				plot_line_low((int[2]){finish[0], finish[1]},
							  (int[2]){begin[0], begin[1]}, color, app);
			else
				plot_line_low((int[2]){begin[0], begin[1]},
							  (int[2]){finish[0], finish[1]}, color, app);
		}
		else
		{
			if (begin[1] > finish[1])
				plot_line_high((int[2]){finish[0], finish[1]},
							   (int[2]){begin[0], begin[1]}, color, app);
			else
				plot_line_high((int[2]){begin[0], begin[1]},
							   (int[2]){finish[0], finish[1]}, color, app);
		}
	}
}

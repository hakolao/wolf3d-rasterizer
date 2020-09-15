/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_drawing_upper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:51:22 by veilo             #+#    #+#             */
/*   Updated: 2020/09/14 17:51:23 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void plot_line_low(t_ir p1, t_ir p2, uint32_t color, t_wolf3d *app)
{
	int dxy[2];
	int dyi[2];
	int xy[2];

	dxy[0] = p2.a - p1.a;
	dxy[1] = p2.b - p1.b;
	dyi[1] = 1;
	if (dxy[1] < 0)
	{
		dyi[1] = -1;
		dxy[1] = -dxy[1];
	}
	dyi[0] = 2 * dxy[1] - dxy[0];
	xy[1] = p1.b;
	xy[0] = p1.a - 1;
	while (++xy[0] <= p2.a)
	{
		plot_pixel_upper(app, app->main_window->rbuffer, xy, color);
		if (dyi[0] > 0)
		{
			xy[1] += dyi[1];
			dyi[0] -= 2 * dxy[0];
		}
		dyi[0] += 2 * dxy[1];
	}
}

static void plot_line_high(t_ir p1, t_ir p2, uint32_t color, t_wolf3d *app)
{
	int dxy[2];
	int dxi[2];
	int xy[2];

	dxy[0] = p2.a - p1.a;
	dxy[1] = p2.b - p1.b;
	dxi[1] = 1;
	if (dxy[0] < 0)
	{
		dxi[1] = -1;
		dxy[0] = -dxy[0];
	}
	dxi[0] = 2 * dxy[0] - dxy[1];
	xy[0] = p1.a;
	xy[1] = p1.b - 1;
	while (++xy[1] <= p2.b)
	{
		plot_pixel_upper_high(app, app->main_window->rbuffer, xy, color);
		if (dxi[0] > 0)
		{
			xy[0] += dxi[1];
			dxi[0] -= 2 * dxy[1];
		}
		dxi[0] += 2 * dxy[0];
	}
}

static void plot_vertical(t_ir begin, t_ir finish, uint32_t color, t_wolf3d *app)
{
	if (begin.b <= finish.b)
	{
		begin.b--;
		while (begin.b <= finish.b)
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){begin.a, begin.b}, color);
			plot_pixel(app, app->main_window->rbuffer, (int[2]){begin.a + 1, begin.b}, color / 2);
			begin.b++;
		}
		plot_pixel(app, app->main_window->rbuffer, (int[2]){begin.a + 1, begin.b}, color / 2);
	}
	else if (begin.b >= finish.b)
	{
		finish.b--;
		while (begin.b >= finish.b)
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){finish.a, finish.b}, color);
			plot_pixel(app, app->main_window->rbuffer, (int[2]){finish.a - 1, finish.b}, color / 2);
			finish.b++;
		}
		plot_pixel(app, app->main_window->rbuffer, (int[2]){finish.a - 1, finish.b}, color / 2);
	}
}

static void plot_horizontal(t_ir begin, t_ir finish, uint32_t color, t_wolf3d *app)
{
	if (begin.a <= finish.a)
	{
		begin.a--;
		while (begin.a <= finish.a)
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){begin.a, begin.b}, color);
			plot_pixel(app, app->main_window->rbuffer, (int[2]){begin.a, begin.b - 1}, color / 2);
			begin.a++;
		}
		plot_pixel(app, app->main_window->rbuffer, (int[2]){begin.a, begin.b - 1}, color / 2);
	}
	else if (begin.a >= finish.a)
	{
		finish.a--;
		while (begin.a >= finish.a)
		{
			plot_pixel(app, app->main_window->rbuffer, (int[2]){finish.a, finish.b}, color);
			plot_pixel(app, app->main_window->rbuffer, (int[2]){finish.a, finish.b + 1}, color / 2);
			finish.a++;
		}
		plot_pixel(app, app->main_window->rbuffer, (int[2]){finish.a, finish.b + 1}, color / 2);
	}
}

void draw_line_upper(t_ir begin, t_ir finish, uint32_t color, t_wolf3d *app)
{
	if (begin.a == finish.a)
		plot_vertical(begin, finish, color, app);
	else if (begin.b == finish.b)
		plot_horizontal(begin, finish, color, app);
	else
	{
		if (ft_abs(finish.b - begin.b) < ft_abs(finish.a - begin.a))
		{
			if (begin.a > finish.a)
				plot_line_low((t_ir){.a = finish.a, .b = finish.b},
							  (t_ir){.a = begin.a, .b = begin.b}, color, app);
			else
				plot_line_low((t_ir){.a = begin.a, .b = begin.b},
							  (t_ir){.a = finish.a, .b = finish.b}, color, app);
		}
		else
		{
			if (begin.b > finish.b)
				plot_line_high((t_ir){.a = finish.a, .b = finish.b},
							   (t_ir){.a = begin.a, .b = begin.b}, color, app);
			else
				plot_line_high((t_ir){.a = begin.a, .b = begin.b},
							   (t_ir){.a = finish.a, .b = finish.b}, color, app);
		}
	}
}

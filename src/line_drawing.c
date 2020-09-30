/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_drawing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:51:22 by veilo             #+#    #+#             */
/*   Updated: 2020/09/30 15:50:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		plot_line_low(uint32_t *buffer, uint32_t dimensions_wh[2],
				int32_t edge[2][2], uint32_t color)
{
	int		dxy[2];
	int		dyi[2];
	int		xy[2];

	dxy[0] = edge[1][0] - edge[0][0];
	dxy[1] = edge[1][1] - edge[0][1];
	dyi[1] = 1;
	if (dxy[1] < 0)
	{
		dyi[1] = -1;
		dxy[1] = -dxy[1];
	}
	dyi[0] = 2 * dxy[1] - dxy[0];
	xy[1] = edge[0][1];
	xy[0] = edge[0][0] - 1;
	while (++xy[0] <= edge[1][0])
	{
		l3d_pixel_plot(buffer, dimensions_wh, xy, color);
		if (dyi[0] > 0)
		{
			xy[1] += dyi[1];
			dyi[0] -= 2 * dxy[0];
		}
		dyi[0] += 2 * dxy[1];
	}
}

static void		plot_line_high(uint32_t *buffer, uint32_t dimensions_wh[2],
				int32_t edge[2][2], uint32_t color)
{
	int dxy[2];
	int dxi[2];
	int xy[2];

	dxy[0] = edge[1][0] - edge[0][0];
	dxy[1] = edge[1][1] - edge[0][1];
	dxi[1] = 1;
	if (dxy[0] < 0)
	{
		dxi[1] = -1;
		dxy[0] = -dxy[0];
	}
	dxi[0] = 2 * dxy[0] - dxy[1];
	xy[0] = edge[0][0];
	xy[1] = edge[0][1] - 1;
	while (++xy[1] <= edge[1][1])
	{
		l3d_pixel_plot(buffer, dimensions_wh, xy, color);
		if (dxi[0] > 0)
		{
			xy[0] += dxi[1];
			dxi[0] -= 2 * dxy[1];
		}
		dxi[0] += 2 * dxy[0];
	}
}

static void		plot_vertical(uint32_t *buffer, uint32_t dimensions_wh[2],
				int32_t edge[2][2], uint32_t color)
{
	if (edge[0][1] <= edge[1][1])
	{
		edge[0][1]--;
		while (edge[0][1] <= edge[1][1])
		{
			l3d_pixel_plot(buffer, dimensions_wh, edge[0], color);
			edge[0][1]++;
		}
	}
	else if (edge[0][1] >= edge[1][1])
	{
		edge[1][1]--;
		while (edge[0][1] >= edge[1][1])
		{
			l3d_pixel_plot(buffer, dimensions_wh, edge[1], color);
			edge[1][1]++;
		}
	}
}

static void		plot_horizontal(uint32_t *buffer, uint32_t dimensions_wh[2],
				int32_t edge[2][2], uint32_t color)
{
	if (edge[0][0] <= edge[1][0])
	{
		edge[0][0]--;
		while (edge[0][0] <= edge[1][0])
		{
			l3d_pixel_plot(buffer, dimensions_wh, edge[0], color);
			edge[0][0]++;
		}
	}
	else if (edge[0][0] >= edge[1][0])
	{
		edge[1][0]--;
		while (edge[0][0] >= edge[1][0])
		{
			l3d_pixel_plot(buffer, dimensions_wh, edge[1], color);
			edge[1][0]++;
		}
	}
}

void			draw_line(uint32_t *buffer, uint32_t dimensions_wh[2],
				int32_t edge[2][2], uint32_t color)
{
	if (edge[0][0] == edge[1][0])
		plot_vertical(buffer, dimensions_wh, edge, color);
	else if (edge[0][1] == edge[1][1])
		plot_horizontal(buffer, dimensions_wh, edge, color);
	else
	{
		if (ft_abs(edge[1][1] - edge[0][1]) < ft_abs(edge[1][0] - edge[0][0]))
		{
			if (edge[0][0] > edge[1][0])
				plot_line_low(buffer, dimensions_wh,
					(int32_t[2][2]){{edge[1][0], edge[1][1]},
						{edge[0][0], edge[0][1]}}, color);
			else
				plot_line_low(buffer, dimensions_wh,
					(int32_t[2][2]){{edge[0][0], edge[0][1]},
						{edge[1][0], edge[1][1]}}, color);
		}
		else
		{
			if (edge[0][1] > edge[1][1])
				plot_line_high(buffer, dimensions_wh,
					(int32_t[2][2]){{edge[1][0], edge[1][1]},
						{edge[0][0], edge[0][1]}}, color);
			else
				plot_line_high(buffer, dimensions_wh,
					(int32_t[2][2]){{edge[0][0], edge[0][1]},
						{edge[1][0], edge[1][1]}}, color);
		}
	}
}

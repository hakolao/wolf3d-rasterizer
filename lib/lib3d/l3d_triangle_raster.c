/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_raster.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:02:18 by veilo             #+#    #+#             */
/*   Updated: 2020/10/13 19:09:32 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		order_corners_y(t_triangle *triangle, t_vertex **vtc,
								t_vec2 *ordered_corners, t_vec2 *points_2d)
{
	size_t	indices[3];

	ft_min_double_idx((double[3]){points_2d[0][1],
									points_2d[1][1],
									points_2d[2][1]},
						3, &(indices[0]));
	vtc[0] = triangle->vtc[indices[0]];
	ml_vector2_copy(points_2d[indices[0]], ordered_corners[0]);
	ft_max_double_idx((double[3]){points_2d[0][1],
									points_2d[1][1],
									points_2d[2][1]},
					  3, &(indices[2]));
	vtc[2] = triangle->vtc[indices[2]];
	ml_vector2_copy(points_2d[indices[2]], ordered_corners[2]);
	indices[1] = 3 - (indices[0] + indices[2]);
	vtc[1] = triangle->vtc[indices[1]];
	ml_vector2_copy(points_2d[indices[1]], ordered_corners[1]);
}

static void		scan_line(uint32_t *buffer, uint32_t *dimensionswh,
							float *limits, t_triangle *triangle)
{
	int		width;
	int		height;
	int		x;
	int		y;
	int		end_x;

	width = dimensionswh[0];
	height = dimensionswh[1];
	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		l3d_pixel_plot(buffer,
						(uint32_t[2]){width, height},
						(int[2]){x + width / 2, y + height / 2},
						l3d_triangle_normal_color(triangle));
		x++;
	}
}

static void		raster_upper(uint32_t *buffer, uint32_t *dimensionswh,
							t_triangle *triangle, t_raster_data *data)
{
	float	x;
	float	y;
	float	end_x;

	y = data->y1;
	while (y < data->y2)
	{
		x = data->x2 + data->slope_ab * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (x < end_x)
			scan_line(buffer, dimensionswh,
						(float[3]){x, end_x + 1, y}, triangle);
		else
			scan_line(buffer, dimensionswh,
						(float[3]){end_x, x + 1, y}, triangle);
		y++;
	}
}

static void		raster_lower(uint32_t *buffer, uint32_t *dimensionswh,
							t_triangle *triangle, t_raster_data *data)
{
	float	x;
	float	y;
	float	end_x;

	y = data->y2;
	while (y < data->y3)
	{
		x = data->x2 + data->slope_bc * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (x < end_x)
			scan_line(buffer, dimensionswh,
						(float[3]){x, end_x + 1, y}, triangle);
		else
			scan_line(buffer, dimensionswh,
						(float[3]){end_x, x + 1, y}, triangle);
		y++;
	}
}

void			l3d_triangle_raster(uint32_t *buffer, uint32_t *dimensions,
									t_triangle *triangle, t_vec2 *points_2d)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	order_corners_y(triangle, triangle->ordered_vtc, ordered_points_2d,
					points_2d);
	data.x1 = floor(ordered_points_2d[0][0]);
	data.x2 = floor(ordered_points_2d[1][0]);
	data.x3 = floor(ordered_points_2d[2][0]);
	data.y1 = floor(ordered_points_2d[0][1]);
	data.y2 = floor(ordered_points_2d[1][1]);
	data.y3 = floor(ordered_points_2d[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	raster_upper(buffer, dimensions, triangle, &data);
	raster_lower(buffer, dimensions, triangle, &data);
}

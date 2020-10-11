/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_rasterization.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:02:18 by veilo             #+#    #+#             */
/*   Updated: 2020/10/06 17:02:18 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		order_corners_y(t_triangle *triangle, t_vertex **vtc,
								t_vec2 *ordered_corners, t_vec2 *points_2d)
{
	size_t indices[3];

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
	int width = dimensionswh[0];
	int height = dimensionswh[1];
	int x;
	int y;
	int end_x;

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
	//!use triangle->ordered_vtc
}

static void		raster_upper(uint32_t *buffer, uint32_t *dimensionswh,
							t_triangle *triangle, t_raster_data *data)
{
	float x;
	float y;
	float end_x;

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
	float x;
	float y;
	float end_x;

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
	t_raster_data data;
	t_vec2 ordered_points_2d[3];

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

void			l3d_calculate_bary_coords(t_vec2 *points_2d, t_vec2 point,
											float *barycoords)
{
	float	ax = points_2d[0][0];
	float	ay = points_2d[0][1];
	float	bx = points_2d[1][0];
	float	by = points_2d[1][1];
	float	cx = points_2d[2][0];
	float	cy = points_2d[2][1];
	float	px = point[0];
	float	py = point[1];

	barycoords[0] = ((by - cy) * (px - cx) + (cx - bx) * (py - cy)) /
					((by - cy) * (ax - cx) + (cx - bx) * (ay - cy));
	barycoords[1] = ((cy - ay) * (px - cx) + (ax - cx) * (py - cy)) /
					((by - cy) * (ax - cx) + (cx - bx) * (ay - cy));
	barycoords[2] = 1 - barycoords[0] - barycoords[1];
}

void			l3d_interpolate_uv(t_triangle *triangle, float *barycoords,
									t_vec2 point_uv)
{
	float	Auv_x = triangle->vtc[0]->uv[0];
	float	Auv_y = triangle->vtc[0]->uv[1];
	float	Buv_x = triangle->vtc[1]->uv[0];
	float	Buv_y = triangle->vtc[1]->uv[1];
	float	Cuv_x = triangle->vtc[2]->uv[0];
	float	Cuv_y = triangle->vtc[2]->uv[1];

	point_uv[0] = barycoords[0] * Auv_x + barycoords[1] * Buv_x + barycoords[2] * Cuv_x;
	point_uv[1] = barycoords[0] * Auv_y + barycoords[1] * Buv_y + barycoords[2] * Cuv_y;
}

uint32_t		l3d_sample_texture(uint32_t *texture_data, int width,
									int height, t_vec2 uv_point)
{
	return (texture_data[(int)(floor(uv_point[0] * width
						+ width * (uv_point[1] * height)))]);
}
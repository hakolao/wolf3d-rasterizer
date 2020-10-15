/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_raster.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:02:18 by veilo             #+#    #+#             */
/*   Updated: 2020/10/15 18:01:56 by ohakola          ###   ########.fr       */
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

static void		clamp_bary(float *barycoords)
{
	static int i = 0;
	if (barycoords[0] > 1.0)
		barycoords[0] = 1.0;
	if (barycoords[1] > 1.0)
		barycoords[1] = 1.0;
	if (barycoords[2] > 1.0)
		barycoords[2] = 1.0;
	if (barycoords[0] < 0.0)
		barycoords[0] = 0.0;
	if (barycoords[1] < 0.0)
		barycoords[1] = 0.0;
	if (barycoords[2] < 0.0)
		barycoords[2] = 0.0;
	if (i < 1)
		ft_printf("barycoords clamped\n");
	i = 1;
}

static void		scan_line(uint32_t *buffer, uint32_t *dimensionswh,
							float *limits, t_triangle *triangle)
{
	int			width = dimensionswh[0];
	int			height = dimensionswh[1];
	int			x;
	int			y;
	int			end_x;
	float		barycoords[3];
	t_vec2		point_uv;
	uint32_t	color;
	ft_memset(barycoords, 0, sizeof(float) * 3);
	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		l3d_calculate_bary_coords(triangle->points_2d, (t_vec2){x, y}, barycoords);//! this gives bs
		//!either the formula is wrong (unlikely) or the points2d are wrong or x and y are wrong
		clamp_bary(barycoords);
		l3d_interpolate_uv(triangle, barycoords, point_uv);
		// if (point_uv[0] > 1.0 || point_uv[1] > 1.0 || point_uv[0] < 0.0 || point_uv[1] < 0.0)
			// ft_printf("U: %f V: %f", point_uv[0], point_uv[1]);

		color = l3d_sample_texture(triangle->material->texture,
								   triangle->material->width,
								   triangle->material->height,
								   point_uv);
		// ft_printf("color: %x \n", color);
		// color = 0xffaaffff;
		l3d_pixel_plot(buffer,
					   (uint32_t[2]){width, height},
					   (int[2]){x + width / 2, y + height / 2},
					   color);
		// if (barycoords[0] < 0 || barycoords[1] < 0 || barycoords[2] < 0)
		// {
		// 	l3d_pixel_plot(buffer,
		// 				   (uint32_t[2]){width, height},
		// 				   (int[2]){x + width / 2, y + height / 2},
		// 				   0xffff11ff);
		// }
		// if (barycoords[0] > 1.1 || barycoords[1] > 1.1 || barycoords[2] > 1.1)
		// {
		// 	l3d_pixel_plot(buffer,
		// 				   (uint32_t[2]){width, height},
		// 				   (int[2]){x + width / 2, y + height / 2},
		// 				   0x33aa11ff);
		// }
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
									t_triangle *triangle)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	order_corners_y(triangle, triangle->ordered_vtc, ordered_points_2d,
					triangle->points_2d);
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

/*
**	Calculates the barycentric coordinates for a 2d point
*/

void			l3d_calculate_bary_coords(t_vec2 *triangle_points_2d, t_vec2 point,
											float *barycoords)
{
	// float	ax = triangle_points_2d[0][0];
	// float	ay = triangle_points_2d[0][1];
	// float	bx = triangle_points_2d[1][0];
	// float	by = triangle_points_2d[1][1];
	// float	cx = triangle_points_2d[2][0];
	// float	cy = triangle_points_2d[2][1];
	// float	px = point[0];
	// float	py = point[1];

	// Point p, Point a, Point b, Point c, float &u, float &v, float &w)
	t_vec2 a;
	ml_vector2_copy(triangle_points_2d[0], a);
	t_vec2 b;
	ml_vector2_copy(triangle_points_2d[1], b);
	t_vec2 c;
	ml_vector2_copy(triangle_points_2d[2], c);
	t_vec2 v0;
	t_vec2 v1;
	t_vec2 v2;
	ml_vector2_sub(b, a, v0);
	ml_vector2_sub(c, a, v1);
	ml_vector2_sub(point, a, v2);
	float den = v0[0] * v1[1] - v1[0] * v0[1];
	float v = (v2[0] * v1[1] - v1[0] * v2[1]) / den;
	float w = (v0[0] * v2[1] - v2[0] * v0[1]) / den;
	float u = 1.0f - v - w;
	barycoords[0] = u;
	barycoords[1] = v;
	barycoords[2] = w;

	//!precalculate at least the denominator
	// barycoords[0] = ((by - cy) * (px - cx) + (cx - bx) * (py - cy)) /
	// 				((by - cy) * (ax - cx) + (cx - bx) * (ay - cy));
	// barycoords[1] = ((cy - ay) * (px - cx) + (ax - cx) * (py - cy)) /
	// 				((by - cy) * (ax - cx) + (cx - bx) * (ay - cy));
	// barycoords[2] = 1 - barycoords[0] - barycoords[1];

	// if (barycoords[0] < 0 || barycoords[1] < 0 || barycoords[2] < 0)
	// {
	// 	ft_printf("barycoords was negative: %f | %f | %f\n",
	// 			  barycoords[0], barycoords[1], barycoords[2]);
	// 	ft_printf("point: %f | %f \n",
	// 			  point[0], point[1]);
	// }
	// if (barycoords[0] > 1.1 || barycoords[1] > 1.1 || barycoords[2] > 1.1)
	// {
	// 	ft_printf("barycoords was over 1.1: %f | %f | %f\n",
	// 			  barycoords[0], barycoords[1], barycoords[2]);
	// 	ft_printf("point: %f | %f \n",
	// 			  point[0], point[1]);
	// }
}

/*
**	Interpolates the uv coordinates for a 2d point based on barycentric
**	coordinates
*/

void			l3d_interpolate_uv(t_triangle *triangle, float *barycoords,
									t_vec2 point_uv)
{
	float	Auv_x = triangle->uvs[0][0];
	float	Auv_y = triangle->uvs[0][1];
	float	Buv_x = triangle->uvs[1][0];
	float	Buv_y = triangle->uvs[1][1];
	float	Cuv_x = triangle->uvs[2][0];
	float	Cuv_y = triangle->uvs[2][1];

	point_uv[0] = (barycoords[0] * Auv_x + barycoords[1] *
					Buv_x + barycoords[2] * Cuv_x);
	point_uv[1] = (barycoords[0] * Auv_y + barycoords[1] *
					Buv_y + barycoords[2] * Cuv_y);
	// if (point_uv[0] < 0.0 || point_uv[1] < 0.0)
	// 	ft_printf("uv was negative\n");//!was negative
}

/*
**	Samples the texture with given uv_coordinates
*/

uint32_t		l3d_sample_texture(uint32_t *texture_data, int width,
									int height, t_vec2 uv_point)
{
	// static float max = 0;
	// ft_printf("U value from texture: %f\n", (float)(uv_point[0]));
	// ft_printf("V value from texture: %f\n", (float)((uv_point[1])));
	if ((int)(floor(uv_point[0] * width +
					width * (uv_point[1] * height))) > 65536)
	{
		ft_printf("uvs: %f  %f\n", uv_point[0], uv_point[1]);
		ft_printf("x value from texture: %d\n", (int)(uv_point[0] * width));
		ft_printf("y value from texture: %d\n", (int)(width * (uv_point[1] * height)));
	}//! y value gets out of control when this happens_> uv[1]
	// ft_printf("color in the position: %u\n", texture_data[(int)(floor(uv_point[0] * width +
	// 					width * (uv_point[1] * height)))]);
	float x = uv_point[0] * width;
	float y = uv_point[1] * height;
	float index = x + width * y;
	// if (uv_point[0] > max)
	// 	max = uv_point[0];
	// ft_printf("max index: %f\n", max);
	return (texture_data[(int)floor(index)]);
	//TODO MAKE AN UV MAP WITH STRIPES/SQUARES AND SEE HOW IT MAPS TO THE OBJECT
	//TODO COLOR THE BARYCETRIC CENTER OF A TRIANGLE AND SEE IF IT MAPS CORRECTLY
	//TODO MAP THE CORNERS USING BARYCENTRIC / MAP THE EDGES
}
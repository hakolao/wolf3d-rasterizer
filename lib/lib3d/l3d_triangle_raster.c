/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_raster.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 17:02:18 by veilo             #+#    #+#             */
/*   Updated: 2020/10/15 20:12:16 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		order_corners_y(t_triangle *triangle, t_vertex **vtc,
								t_vec2 *ordered_corners, t_vec2 *points_2d)
{
	size_t	indices[3];
	t_vec2	uvs[3];

	ml_vector2_copy(triangle->uvs[0], uvs[0]);
	ml_vector2_copy(triangle->uvs[1], uvs[1]);
	ml_vector2_copy(triangle->uvs[2], uvs[2]);
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

	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		// ml_vector3_print(triangle->vtc[0]->pos);
		// ml_vector3_print(triangle->vtc[1]->pos);
		// ml_vector3_print(triangle->vtc[2]->pos);
		// ft_printf("x: %d y: %d ", x, y);
		l3d_calculate_bary_coords(triangle, triangle->points_2d, (t_vec2){x, y}, barycoords);
		
		clamp_bary(barycoords);
		l3d_interpolate_uv(triangle, barycoords, point_uv);
		color = l3d_sample_texture(triangle->material->texture,
								   triangle->material->width,
								   triangle->material->height,
									point_uv);
		l3d_pixel_plot(buffer,
					   (uint32_t[2]){width, height},
					   (int[2]){x + width / 2, y + height / 2},
					   color);
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

void			l3d_calculate_bary_coords(t_triangle *triangle, t_vec2 *triangle_points_2d, t_vec2 point,
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
	(void)triangle;
	// t_vec3 X;
	// t_vec3 Y;
	// t_vec3 Z;
	// ml_vector3_sub((t_vec3){point[0], point[1], z}, triangle->vtc[0]->pos, X);
	// ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, Y);
	// ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, Z);

	// ft_printf("z: %f\n", z);
	// barycoords[1] = (X[1] * Z[2] - X[2] * Z[1]) / (Y[1] * Z[2] - Y[2] * Z[1]);
	// barycoords[2] = (-X[1] * Y[2] + X[2] * Y[1]) / (Y[1] * Z[2] - Y[2] * Z[1]);
	// barycoords[0] = 1 - barycoords[1] - barycoords[2];

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
	// float	Auv_x = triangle->uvs[0][0];
	// float	Auv_y = triangle->uvs[0][1];
	// float	Buv_x = triangle->uvs[1][0];
	// float	Buv_y = triangle->uvs[1][1];
	// float	Cuv_x = triangle->uvs[2][0];
	// float	Cuv_y = triangle->uvs[2][1];

	float Auv_x = 0.0;
	float Auv_y = 0.0;
	float Buv_x = 0.0;
	float Buv_y = 1.0;
	float Cuv_x = 1.0;
	float Cuv_y = 0.5;

	// float z = (barycoords[0] * triangle->vtc[0]->pos[2] +
	// 		   barycoords[1] * triangle->vtc[1]->pos[2] +
	// 		   barycoords[2] * triangle->vtc[2]->pos[2]);
	// float minz = ft_max_double((double[3]){triangle->vtc[0]->pos[2],
	// 									 triangle->vtc[1]->pos[2],
	// 									 triangle->vtc[2]->pos[2]}, 3);
	// z -= minz;
	// z = fabs(z);
	t_vec3 A;
	ml_vector3_copy(triangle->vtc[0]->pos, A);
	t_vec3 B;
	ml_vector3_copy(triangle->vtc[1]->pos, B);
	t_vec3 C;
	ml_vector3_copy(triangle->vtc[2]->pos, C);
	
	float z = (barycoords[0] * A[2] +
			   barycoords[1] * B[2] +
			   barycoords[2] * C[2]);
	// float distance = z - triangle->center[2];
	float max = ft_max_double((double[3]){triangle->vtc[0]->pos[2], triangle->vtc[1]->pos[2],
							  triangle->vtc[2]->pos[2]}, 3);
	float min = ft_min_double((double[3]){triangle->vtc[0]->pos[2], triangle->vtc[1]->pos[2],
							  triangle->vtc[2]->pos[2]}, 3);
							  
	z /= max;
	// ft_printf("z  %f max %f min %f fraction %f\n", z, max, min, fraction);
	// float centerz = (triangle->vtc[0]->pos[2] +
	// 				triangle->vtc[1]->pos[2] +
	// 				triangle->vtc[2]->pos[2]) / 3;

		// z = fabs(z) / fabs(centerz);
		// z = fabs(z);
	// ft_printf("z: %f\n", z);

	point_uv[0] = (barycoords[0] * Auv_x + barycoords[1] * Buv_x + barycoords[2] * Cuv_x) / z;
	point_uv[1] = (barycoords[0] * Auv_y + barycoords[1] * Buv_y + barycoords[2] * Cuv_y) / z;
	(void)triangle;
	(void)min;
	(void)max;
}

/*
**	Samples the texture with given uv_coordinates
*/

uint32_t		l3d_sample_texture(uint32_t *texture_data, int width,
									int height, t_vec2 uv_point)
{
	int x = floor(uv_point[0] * (width - 1));
	int y = floor(uv_point[1] * (height - 1));
	float index;
	index = x + width * y;
	// int mult = 500;
	// uint32_t result = (int)(fabs(uv_point[0])/mult * 255) << 8;
	// result += 255;
	// result += (int)(fabs(uv_point[0])/mult * 255) << 16;
	// result += (int)(fabs(uv_point[0]) / mult * 255) << 24;
	// return (result);
	return (texture_data[(int)floor(index)]);
	(void)height;
	(void)width;
	(void)x;
	(void)y;
}
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
		l3d_calculate_barycoords(triangle->points_2d, (t_vec2){x, y}, barycoords);
		
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

void			l3d_calculate_barycoords(t_vec2 *triangle_points_2d, t_vec2 point,
										float *barycoords)
{
	float	denominator;

	denominator = ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
					(triangle_points_2d[0][0] - triangle_points_2d[2][0]) +
					(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
					(triangle_points_2d[0][1] - triangle_points_2d[2][1]));

	barycoords[0] = ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
					(point[0] - triangle_points_2d[2][0]) +
					(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
					(point[1] - triangle_points_2d[2][1])) / denominator;
	barycoords[1] = ((triangle_points_2d[2][1] - triangle_points_2d[0][1]) *
					(point[0] - triangle_points_2d[2][0]) +
					(triangle_points_2d[0][0] - triangle_points_2d[2][0]) *
					(point[1] - triangle_points_2d[2][1])) / denominator;
	barycoords[2] = 1 - barycoords[0] - barycoords[1];
}

/*
**	Interpolates the uv coordinates for a 2d point based on barycentric
**	coordinates
*/

void			l3d_interpolate_uv(t_triangle *triangle, float *barycoords,
									t_vec2 point_uv)
{
	float	Auv_x = triangle->uvs[0][0];
	float Auv_y = triangle->uvs[0][1];
	float Buv_x = triangle->uvs[1][0];
	float Buv_y = triangle->uvs[1][1];
	float Cuv_x = triangle->uvs[2][0];
	float Cuv_y = triangle->uvs[2][1];

	t_vec3 A;
	ml_vector3_copy(triangle->vtc[0]->pos, A);
	t_vec3 B;
	ml_vector3_copy(triangle->vtc[1]->pos, B);
	t_vec3 C;
	ml_vector3_copy(triangle->vtc[2]->pos, C);
	point_uv[0] = ((barycoords[0] * Auv_x) / A[2] +
				(barycoords[1] * Buv_x) / B[2] +
				(barycoords[2] * Cuv_x) / C[2]) /
				((barycoords[0] * 1) / A[2] +
				(barycoords[1] * 1) / B[2] +
				(barycoords[2] * 1) / C[2]);
	point_uv[1] = ((barycoords[0] * Auv_y) / A[2] +
				(barycoords[1] * Buv_y) / B[2] +
				(barycoords[2] * Cuv_y) / C[2]) /
				((barycoords[0] * 1) / A[2] +
				(barycoords[1] * 1) / B[2] +
				(barycoords[2] * 1) / C[2]);
}

/*
**	Samples the texture with given uv_coordinates
**	x = (floor(U * (width - 1))); //TODO Check later if the -1 offset is needed
**	y = (floor(V * (height - 1)));
**	index = x + width * y;
*/

uint32_t		l3d_sample_texture(uint32_t *texture_data, int width,
									int height, t_vec2 uv_point)
{
	return (texture_data[(int)floor((floor(uv_point[0] * (width - 1))) +
						width * (floor(uv_point[1] * (height - 1))))]);
}
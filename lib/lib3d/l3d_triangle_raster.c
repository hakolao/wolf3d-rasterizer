/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_raster.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 21:11:09 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/29 20:25:05 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		order_corners_y(t_vec2 *ordered_corners, t_vec2 *points_2d)
{
	size_t	indices[3];

	ft_min_double_idx((double[3]){points_2d[0][1],
									points_2d[1][1],
									points_2d[2][1]},
						3, &(indices[0]));
	ml_vector2_copy(points_2d[indices[0]], ordered_corners[0]);
	ft_max_double_idx((double[3]){points_2d[0][1],
									points_2d[1][1],
									points_2d[2][1]},
						3, &(indices[2]));
	ml_vector2_copy(points_2d[indices[2]], ordered_corners[2]);
	indices[1] = 3 - (indices[0] + indices[2]);
	ml_vector2_copy(points_2d[indices[1]], ordered_corners[1]);
}

void			clamp_bary(float *barycoords)
{
	if (barycoords[0] > 1.0)
		barycoords[0] = 1.0;
	else if (barycoords[0] < 0.0)
		barycoords[0] = 0.0;
	if (barycoords[1] > 1.0)
		barycoords[1] = 1.0;
	else if (barycoords[1] < 0.0)
		barycoords[1] = 0.0;
	if (barycoords[2] > 1.0)
		barycoords[2] = 1.0;
	else if (barycoords[2] < 0.0)
		barycoords[2] = 0.0;
}

void			clamp_uv(t_vec2 uv)
{
	if (uv[0] > 1.0)
		uv[0] = 1.0;
	else if (uv[0] < 0.0)
		uv[0] = 0.0;
	if (uv[1] > 1.0)
		uv[1] = 1.0;
	else if (uv[1] < 0.0)
		uv[1] = 0.0;
}

static float	calculate_z_val(float baryc[3], t_triangle *triangle)
{
	return (((baryc[0] * triangle->vtc_zvalue[0] +
			baryc[1] * triangle->vtc_zvalue[1] +
			baryc[2] * triangle->vtc_zvalue[2])));
}

void		normal_from_color(uint32_t color, t_vec3 normal)
{
	uint32_t	rgba[4];

	l3d_u32_to_rgba(color, rgba);
	normal[0] = (float)rgba[0] * 0.00392156862745098;
	normal[1] = (float)rgba[1] * 0.00392156862745098;
	normal[2] = (float)rgba[2] * 0.00392156862745098;

	(void)normal;
	(void)color;
	(void)rgba;
}

void		calc_bumped_normal(t_triangle *triangle, t_vec2 uv, t_vec3 res)
{
	uint32_t	normal_value;
	t_vec3		bumpnormal;
	t_vec3		resultnormal;
	t_mat3		tbn;

	normal_value = l3d_sample_texture(triangle->material->normal_map, uv);
	normal_from_color(normal_value, bumpnormal);
	ml_vector3_mul(bumpnormal, 2, bumpnormal);
	ml_vector3_sub(bumpnormal, (t_vec3){1.0, 1.0, 1.0}, bumpnormal);
	ml_matrix3_column(triangle->tangent, triangle->bitangent,
						triangle->normalized_normal, tbn);
	ml_matrix3_mul_vec3(tbn, bumpnormal, resultnormal);
	ml_vector3_normalize(resultnormal, res);
	(void)triangle;
	(void)uv;
	(void)res;
}

uint32_t		fragment_shade_normal(t_vec3 light_vector, t_vec3 frag_normal,
									uint32_t frag)
{
	float		dot;
	uint32_t	rgba[4];
	int			i;

	i = -1;
	dot = ml_vector3_dot(light_vector, frag_normal);
	l3d_u32_to_rgba(frag, rgba);
	while (++i < 3)
	{
		rgba[i] *= fabs(dot);
	}
	rgba[3] = 255;
	return (l3d_rgba_to_u32(rgba));
}

uint32_t pixel_normal_shaded(uint32_t pixel, t_triangle *triangle, t_vec2 uv)
{
	t_vec3 frag_normal;
	t_vec3 light_vector;

	ml_vector3_set(light_vector, 0.0, 0.0, -1.0);
	// ml_vector3_set_all(frag_normal, 0.0);
	calc_bumped_normal(triangle, uv, frag_normal);
	(void)triangle;
	(void)uv;
	return (fragment_shade_normal(light_vector, frag_normal, pixel));
}

static uint32_t	pixel_depth_shaded(uint32_t pixel, float z_val)
{
	float	intensity;

	intensity = 10.0;
	return (l3d_color_blend_u32(pixel, 0x000000ff,
		1.0 - (ft_abs(z_val) * intensity)));
}

static void		draw_pixel(t_sub_framebuffer *buffers, int32_t xy[2],
							t_triangle *triangle)
{
	float		baryc[3];
	t_vec2		uv;
	float		z_val;
	int32_t		offset_xy[2];
	uint32_t	pixel;

	offset_xy[0] = xy[0] + buffers->x_offset;
	offset_xy[1] = xy[1] + buffers->y_offset;
	l3d_calculate_barycoords(triangle->points_2d, (t_vec2){xy[0], xy[1]}, baryc);
	z_val = calculate_z_val(baryc, triangle);
	if (z_val <= l3d_pixel_get_float(buffers->zbuffer, (uint32_t[2]){
		buffers->width, buffers->height}, offset_xy))
	{
		l3d_interpolate_uv(triangle, baryc, uv);
		clamp_uv(uv);
		pixel = L3D_DEFAULT_COLOR;
		// Shade texture first
		if (triangle->material)
			pixel = l3d_sample_texture(triangle->material->texture, uv);
		// If texture has alpha value of 0, don't color the pixel
		// It was also ignored in zbuffer
		if ((triangle->material->shading_opts & e_shading_zero_alpha) &&
			(pixel & 255) == 0)
			return ;
		//Normal map shading
		if (triangle->material->shading_opts & e_shading_normal_map)
		{
			pixel = pixel_normal_shaded(pixel, triangle, uv);
		}
		// Shade depth
		if (triangle->material->shading_opts & e_shading_depth)
			pixel = pixel_depth_shaded(pixel, z_val);
		// Plot pixel
		l3d_pixel_plot(buffers->buffer, (uint32_t[2]){buffers->width,
				buffers->height}, offset_xy, pixel);
	}
}

static void		scan_line(t_sub_framebuffer *buffers,
							float *limits, t_triangle *triangle)
{
	int			x;
	int			y;
	int			end_x;

	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		if (x + buffers->x_offset < 0)
		{
			x = -buffers->x_offset;
			continue ;
		}
		else if (x + buffers->x_offset >= buffers->width)
			break;
		draw_pixel(buffers, (int32_t[2]){x, y}, triangle);
		x++;
	}
}

static t_bool	is_zero_alpha(t_triangle *triangle, float baryc[3])
{
	t_vec2		uv;

	if (triangle->material &&
		(triangle->material->shading_opts & e_shading_zero_alpha) &&
		triangle->material->texture)
	{
		l3d_interpolate_uv(triangle, baryc, uv);
		clamp_uv(uv);
		if ((l3d_sample_texture(triangle->material->texture, uv) & 255) == 0)
			return (true);
	}
	return (false);
}

static void		draw_zpixel(t_sub_framebuffer *buffers,
								int32_t xy[2], t_triangle *triangle)
{
	float		pixel;
	float		baryc[3];
	float		z_val;
	int32_t		offset_xy[2];

	offset_xy[0] = xy[0] + buffers->x_offset;
	offset_xy[1] = xy[1] + buffers->y_offset;
	l3d_calculate_barycoords(triangle->points_2d, (t_vec2){xy[0], xy[1]}, baryc);
	pixel = l3d_pixel_get_float(buffers->zbuffer, (uint32_t[2]){
		buffers->width, buffers->height
	}, offset_xy);
	if (is_zero_alpha(triangle, baryc))
		return ;
	z_val = calculate_z_val(baryc, triangle);
	if (z_val <= pixel)
	{
		l3d_pixel_plot_float(buffers->zbuffer,
			(uint32_t[2]){buffers->width, buffers->height},
			offset_xy, z_val);
	}
}

static void		scan_z_line(t_sub_framebuffer *buffers,
								float *limits, t_triangle *triangle)
{
	int			x;
	int			y;
	int			end_x;

	y = floor(limits[2]);
	x = floor(limits[0]);
	end_x = floor(limits[1]);
	while (x < end_x)
	{
		if (x + buffers->x_offset < 0)
		{
			x = -buffers->x_offset;
			continue ;
		}
		else if (x + buffers->x_offset >= buffers->width)
			break;
		draw_zpixel(buffers, (int32_t[2]){x, y}, triangle);
		x++;
	}
}

static void		raster_upper(t_sub_framebuffer *bufs,
								t_triangle *tri, t_raster_data *data)
{
	float	x;
	float	y;
	float	end_x;

	y = data->y1;
	while (y < data->y2)
	{
		if (y + bufs->y_offset < 0)
		{
			y = -bufs->y_offset;
			continue ;
		}
		else if (y + bufs->y_offset >= bufs->height)
			break;
		x = data->x2 + data->slope_ab * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (!data->is_zbuffer)
		{
			if (x < end_x)
				scan_line(bufs, (float[3]){x, end_x + 1, y}, tri);
			else if (x > end_x)
				scan_line(bufs, (float[3]){end_x, x + 1, y}, tri);
		}
		else
		{
			if (x < end_x)
				scan_z_line(bufs, (float[3]){x, end_x + 1, y}, tri);
			else if (x > end_x)
				scan_z_line(bufs, (float[3]){end_x, x + 1, y}, tri);
		}
		y++;
	}
}

static void		raster_lower(t_sub_framebuffer *bufs,
								t_triangle *tri, t_raster_data *data)
{
	float	x;
	float	y;
	float	end_x;

	y = data->y2;
	while (y < data->y3)
	{
		if (y + bufs->y_offset < 0)
		{
			y = -bufs->y_offset;
			continue ;
		}
		else if (y + bufs->y_offset >= bufs->height)
			break;
		x = data->x2 + data->slope_bc * (y - data->y2);
		end_x = data->x1 + data->slope_ac * (y - data->y1);
		if (!data->is_zbuffer)
		{
			if (x < end_x)
				scan_line(bufs, (float[3]){x, end_x + 1, y}, tri);
			else if (x > end_x)
				scan_line(bufs, (float[3]){end_x, x + 1, y}, tri);
		}
		else
		{
			if (x < end_x)
				scan_z_line(bufs, (float[3]){x, end_x + 1, y}, tri);
			else if (x > end_x)
				scan_z_line(bufs, (float[3]){end_x, x + 1, y}, tri);
		}
		y++;
	}
}

void			l3d_triangle_set_zbuffer(t_sub_framebuffer *buffers, t_triangle *triangle)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	order_corners_y(ordered_points_2d, triangle->points_2d);
	data.x1 = floor(ordered_points_2d[0][0]);
	data.x2 = floor(ordered_points_2d[1][0]);
	data.x3 = floor(ordered_points_2d[2][0]);
	data.y1 = floor(ordered_points_2d[0][1]);
	data.y2 = floor(ordered_points_2d[1][1]);
	data.y3 = floor(ordered_points_2d[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	data.is_zbuffer = true;
	raster_upper(buffers, triangle, &data);
	raster_lower(buffers, triangle, &data);
}

void			l3d_triangle_raster(t_sub_framebuffer *buffers, t_triangle *triangle)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	order_corners_y(ordered_points_2d, triangle->points_2d);
	data.x1 = floor(ordered_points_2d[0][0]);
	data.x2 = floor(ordered_points_2d[1][0]);
	data.x3 = floor(ordered_points_2d[2][0]);
	data.y1 = floor(ordered_points_2d[0][1]);
	data.y2 = floor(ordered_points_2d[1][1]);
	data.y3 = floor(ordered_points_2d[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	data.is_zbuffer = false;
	raster_upper(buffers, triangle, &data);
	raster_lower(buffers, triangle, &data);
}

/*
**	Calculates the barycentric coordinates for a 2d point
*/

void			l3d_calculate_barycoords(t_vec2 *triangle_points_2d,
										t_vec2 point,
										float *baryc)
{
	float denom;
	float inv_denom;

	denom = ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
			(triangle_points_2d[0][0] - triangle_points_2d[2][0]) +
			(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
			(triangle_points_2d[0][1] - triangle_points_2d[2][1]));

	if (fabs(denom) < L3D_EPSILON)
	{
		denom =  denom < 0 ? -1.0 * L3D_EPSILON : L3D_EPSILON;
	}
	inv_denom = 1 / denom;
	baryc[0] = ((triangle_points_2d[1][1] - triangle_points_2d[2][1]) *
				(point[0] - triangle_points_2d[2][0]) +
				(triangle_points_2d[2][0] - triangle_points_2d[1][0]) *
				(point[1] - triangle_points_2d[2][1])) * inv_denom;
	baryc[1] = ((triangle_points_2d[2][1] - triangle_points_2d[0][1]) *
				(point[0] - triangle_points_2d[2][0]) +
				(triangle_points_2d[0][0] - triangle_points_2d[2][0]) *
				(point[1] - triangle_points_2d[2][1])) * inv_denom;
	baryc[2] = 1 - baryc[0] - baryc[1];
}

/*
**	Interpolates the uv coordinates for a 2d point based on barycentric
**	coordinates
*/

void			l3d_interpolate_uv(t_triangle *triangle, float *baryc,
									t_vec2 uv)
{
	float	az;
	float	bz;
	float	cz;

	az = 1 / triangle->vtc[0]->pos[2];
	bz = 1 / triangle->vtc[1]->pos[2];
	cz = 1 / triangle->vtc[2]->pos[2];
	uv[0] = ((baryc[0] * triangle->uvs[0][0]) * az +
			(baryc[1] * triangle->uvs[1][0]) * bz +
			(baryc[2] * triangle->uvs[2][0]) * cz) /
			((baryc[0] * 1) * az +
			(baryc[1] * 1) * bz +
			(baryc[2] * 1) * cz);
	uv[1] = 1 - ((baryc[0] * triangle->uvs[0][1]) * az +
			(baryc[1] * triangle->uvs[1][1]) * bz +
			(baryc[2] * triangle->uvs[2][1]) * cz) /
			((baryc[0] * 1) * az +
			(baryc[1] * 1) * bz +
			(baryc[2] * 1) * cz);
}

/*
**	Samples the texture with given uv_coordinates
**	x = (floor(U * (width)));
**	y = (floor(V * (height)));
**	index = x + width * y;
*/

uint32_t		l3d_sample_texture(t_surface *texture,
									t_vec2 uv_point)
{
	int			index;
	float		x;
	float		y;
	uint32_t	default_color;

	default_color = L3D_DEFAULT_COLOR;
	if (!texture->pixels)
		return (default_color);
	x = floor(uv_point[0] * texture->w);
	y = floor(uv_point[1] * texture->h);
	if (x >= (int32_t)texture->w)
		x = (float)(texture->w - 1);
	if (y >= (int32_t)texture->h)
		y = (float)(texture->h - 1);
	index = (int)floor(x) + (int)(floor(y * texture->w));
	if (index >= (int32_t)texture->w * (int32_t)texture->h)
		index = texture->w * texture->h - 1;
	else if (index < 0)
		index = 0;
	return (texture->pixels[index]);
}

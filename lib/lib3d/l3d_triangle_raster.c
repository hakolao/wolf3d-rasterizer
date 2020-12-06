/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_raster.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 17:22:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 18:11:10 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		draw_pixel(t_sub_framebuffer *buffers, int32_t xy[2],
							t_triangle *triangle)
{
	t_vec3		baryc;
	t_vec2		uv;
	float		z_val;
	int32_t		offset_xy[2];
	uint32_t	pixel;

	offset_xy[0] = xy[0] + buffers->x_offset;
	offset_xy[1] = xy[1] + buffers->y_offset;
	l3d_calculate_barycoords(triangle->points_2d, (t_vec2){xy[0], xy[1]}, baryc);
	z_val = l3d_z_val(baryc, triangle);
	if (z_val < l3d_pixel_get_float(buffers->zbuffer, (uint32_t[2]){
		buffers->width, buffers->height}, offset_xy))
	{
		l3d_interpolate_uv(triangle, baryc, uv);
		l3d_clamp_uv(uv);
		pixel = L3D_DEFAULT_COLOR;
		if (triangle->material)
			pixel = l3d_sample_texture(triangle->material->texture, uv);
		if ((triangle->material->shading_opts & e_shading_zero_alpha) &&
			(pixel & 255) == 0)
			return ;
		if (triangle->material->shading_opts & e_shading_normal_map)
			pixel = l3d_pixel_normal_shaded(pixel, triangle, uv);
		if (triangle->material->shading_opts & e_shading_depth)
			pixel = l3d_pixel_depth_shaded(pixel, z_val);
		l3d_pixel_plot(buffers->buffer, (uint32_t[2]){buffers->width,
				buffers->height}, offset_xy, pixel);
		if (!(triangle->material->shading_opts & e_shading_ignore_zpass))
			l3d_pixel_plot_float(buffers->zbuffer,
				(uint32_t[2]){buffers->width, buffers->height},
				offset_xy, z_val);
	}
}

static void		scan_line(t_sub_framebuffer *buffers,
							float *limits, t_triangle *triangle)
{
	int32_t			x;
	int32_t			y;
	int32_t			end_x;

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

static void		raster_upper(t_sub_framebuffer *bufs,
								t_triangle *tri, t_raster_data *data)
{
	float		x;
	float		y;
	float		end_x;

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
		if (x < end_x)
			scan_line(bufs, (float[3]){x, end_x + 1, y}, tri);
		else if (x > end_x)
			scan_line(bufs, (float[3]){end_x, x + 1, y}, tri);
		y++;
	}
}

static void		raster_lower(t_sub_framebuffer *bufs,
								t_triangle *tri, t_raster_data *data)
{
	float		x;
	float		y;
	float		end_x;

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
		if (x < end_x)
			scan_line(bufs, (float[3]){x, end_x + 1, y}, tri);
		else if (x > end_x)
			scan_line(bufs, (float[3]){end_x, x + 1, y}, tri);
		y++;
	}
}

void			l3d_triangle_raster(t_sub_framebuffer *buffers, t_triangle *triangle)
{
	t_raster_data	data;
	t_vec2			ordered_points_2d[3];

	l3d_order_corners_y(ordered_points_2d, triangle->points_2d);
	data.x1 = floor(ordered_points_2d[0][0]);
	data.x2 = floor(ordered_points_2d[1][0]);
	data.x3 = floor(ordered_points_2d[2][0]);
	data.y1 = floor(ordered_points_2d[0][1]);
	data.y2 = floor(ordered_points_2d[1][1]);
	data.y3 = floor(ordered_points_2d[2][1]);
	data.slope_bc = (data.x3 - data.x2) / (data.y3 - data.y2);
	data.slope_ac = (data.x3 - data.x1) / (data.y3 - data.y1);
	data.slope_ab = (data.x2 - data.x1) / (data.y2 - data.y1);
	raster_upper(buffers, triangle, &data);
	raster_lower(buffers, triangle, &data);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 15:06:37 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 19:03:30 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

uint32_t		l3d_rgba_to_u32(uint32_t rgba[4])
{
	return (((rgba[0] & 255) << 24) | ((rgba[1] & 255) << 16) |
		((rgba[2] & 255) << 8) | (rgba[3] & 255));
}

void			l3d_u32_to_rgba(uint32_t color, uint32_t rgba[4])
{
	rgba[0] = (color >> 24) & 255;
	rgba[1] = (color >> 16) & 255;
	rgba[2] = (color >> 8) & 255;
	rgba[3] = (color & 255);
}

uint32_t		l3d_color_blend_u32(uint32_t color1, uint32_t color2,
				float ratio)
{
	float		i_ratio;
	uint32_t	rgba_color1[4];
	uint32_t	rgba_color2[4];
	uint32_t	new_color[4];

	if (ratio > 1.f)
		ratio = 1.f;
	else if (ratio < 0.f)
		ratio = 0.f;
	i_ratio = 1.f - ratio;
	l3d_u32_to_rgba(color1, rgba_color1);
	l3d_u32_to_rgba(color2, rgba_color2);
	if (rgba_color2[3] == 0)
		return (color1);
	new_color[0] = (int)((rgba_color1[0] * i_ratio) + (rgba_color2[0] * ratio));
	new_color[1] = (int)((rgba_color1[1] * i_ratio) + (rgba_color2[1] * ratio));
	new_color[2] = (int)((rgba_color1[2] * i_ratio) + (rgba_color2[2] * ratio));
	new_color[3] = (int)((rgba_color1[3] * i_ratio) + (rgba_color2[3] * ratio));
	return (l3d_rgba_to_u32(new_color));
}

uint32_t		l3d_triangle_normal_color(t_triangle *triangle)
{
	t_vec3		normal;
	uint32_t	color;

	ml_vector3_normalize(triangle->normal, normal);
	color = 0x0;
	color += (uint32_t)(255 * fabs(normal[0])) << 8;
	color += (uint32_t)(255 * fabs(normal[1])) << 16;
	color += (uint32_t)(255 * fabs(normal[2])) << 24;
	return (color);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_print2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 01:36:16 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:36:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector4_print(t_vec4 v)
{
	ft_printf("Vec4[%.2f, %.2f, %.2f, %.2f]\n", v[0], v[1], v[2], v[3]);
}

void		ml_vector3_print(t_vec3 v)
{
	ft_printf("Vec3[%.2f, %.2f, %.2f]\n", v[0], v[1], v[2]);
}

void		ml_vector2_print(t_vec2 v)
{
	ft_printf("Vec2[%.2f, %.2f]\n", v[0], v[1]);
}

void		ml_vector4_to_str(t_vec4 v, char *res)
{
	ft_sprintf(res, "Vec4[%.2f, %.2f, %.2f, %.2f]\n", v[0], v[1], v[2], v[3]);
}

void		ml_vector3_to_str(t_vec3 v, char *res)
{
	ft_sprintf(res, "Vec3[%.2f, %.2f, %.2f]\n", v[0], v[1], v[2]);
}

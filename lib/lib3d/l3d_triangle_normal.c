/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_normal.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 12:50:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/15 12:51:32 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void	l3d_triangle_normal_set(t_triangle *triangle)
{
	ml_vector3_sub(triangle->vtc[1]->position, triangle->vtc[0]->position,
		triangle->ab);
	ml_vector3_sub(triangle->vtc[2]->position, triangle->vtc[0]->position,
		triangle->ac);
	ml_vector3_cross(triangle->ab, triangle->ac, triangle->normal);
}

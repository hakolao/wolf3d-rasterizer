/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_centroid.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 16:58:51 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 16:59:35 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void		l3d_triangle_centroid(t_triangle *triangle)
{
	ml_vector3_copy((t_vec3){
		(triangle->vtc[0]->position[0] + triangle->vtc[1]->position[0] +
		triangle->vtc[2]->position[0]) / 3.0,
		(triangle->vtc[0]->position[1] + triangle->vtc[1]->position[1] +
		triangle->vtc[2]->position[1]) / 3.0,
		(triangle->vtc[0]->position[2] + triangle->vtc[1]->position[2] +
		triangle->vtc[2]->position[2]) / 3.0,
	}, triangle->center);
}
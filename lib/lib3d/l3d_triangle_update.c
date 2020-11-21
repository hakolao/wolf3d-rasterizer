/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 22:30:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:52:45 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Updates triangle centroid, ab, ac and normals
*/

void				l3d_triangle_update(t_triangle *triangle)
{
	ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, triangle->ab);
	ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, triangle->ac);
	l3d_triangle_centroid_update(triangle);
	l3d_triangle_normal_update(triangle);
	l3d_triangle_tangent_update(triangle);
}

void				l3d_triangle_tangent_update(t_triangle *triangle)
{
	t_vec3	temptangent;
	t_vec3	tempnormal;
	float	deltas[4];
	float	fraction;

	deltas[0] = triangle->uvs[1][0] - triangle->uvs[0][0];//DU1
	deltas[1] = triangle->uvs[2][0] - triangle->uvs[0][0];//DU2
	deltas[2] = triangle->uvs[1][1] - triangle->uvs[0][1];//DV1
	deltas[3] = triangle->uvs[2][1] - triangle->uvs[0][1];//DV2
	fraction = 1.0 / (deltas[0] * deltas[3] - deltas[1] * deltas[2]);
	if (isnan(fraction))
		fraction = 1.0;
	triangle->tangent[0] = fraction * (deltas[3] * triangle->ab[0] -
									deltas[2] * triangle->ac[0]);
	triangle->tangent[1] = fraction * (deltas[3] * triangle->ab[1] -
									deltas[3] * triangle->ac[1]);
	triangle->tangent[2] = fraction * (deltas[3] * triangle->ab[2] -
									deltas[2] * triangle->ac[2]);
	ml_vector3_normalize(triangle->tangent, triangle->tangent);
	//	triangle->tangent = normalize(tangent - dot(tangent, normal) * normal);
	ml_vector3_normalize(triangle->normal, tempnormal);
	ml_vector3_mul(tempnormal, ml_vector3_dot(triangle->tangent, tempnormal) , temptangent);
	ml_vector3_sub(triangle->tangent, temptangent, triangle->tangent);
	ml_vector3_normalize(triangle->tangent, triangle->tangent);
	///
	ml_vector3_cross(triangle->tangent, tempnormal, triangle->bitangent);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 22:30:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/02 20:11:03 by ohakola          ###   ########.fr       */
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
}

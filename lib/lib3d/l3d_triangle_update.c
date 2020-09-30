/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_triangle_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 22:30:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:41:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Updates triangle centroid and normals
*/

void				l3d_triangle_update(t_triangle *triangle)
{
	l3d_triangle_centroid_update(triangle);
	l3d_triangle_normal_update(triangle);
}

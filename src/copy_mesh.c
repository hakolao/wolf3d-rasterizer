// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   copy_mesh.cc                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/09/07 17:22:11 by veilo             #+#    #+#             //
//   Updated: 2020/09/07 17:22:12 by veilo            ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "wolf3d.h"

static void copy_vertices(t_mesh *src, t_mesh *dest, uint32_t count)
{
	uint32_t	i;

	i = 0;
	while (i < count)
	{
		*(dest->vtc[i]) = *(src->vtc[i]);
		i++;
	}
}

static void copy_triangles(t_mesh *src, t_mesh *dest, uint32_t count)
{
	uint32_t i;

	i = 0;
	while (i < count)
	{
		*(dest->triangles[i]) = *(src->triangles[i]);
		i++;
	}
}

void		copy_mesh(t_mesh *src, t_mesh *dest)
{
	ml_vector3_copy(src->orientation[0], dest->orientation[0]);
	ml_vector3_copy(src->orientation[1], dest->orientation[1]);
	ml_vector3_copy(src->orientation[2], dest->orientation[2]);
	ml_vector3_copy(src->origin, dest->origin);
	dest->parent_object = src->parent_object;
	dest->shader = src->shader;
	dest->triangle_count = src->vtx_count;
	dest->vtx_count = src->vtx_count;
	dest->bound_box = src->bound_box;
	copy_vertices(src, dest, src->vtx_count);
	copy_triangles(src, dest, src->triangle_count);
}

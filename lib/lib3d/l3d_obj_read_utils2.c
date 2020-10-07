/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_obj_read_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 22:43:43 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/07 22:08:56 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

/*
** Our obj file reader expects triangulated data, thus this validator tests
** number of vertices, textures and normals equals max incides read from obj
** data.
*/

t_bool				l3d_is_valid_obj(t_obj *obj)
{
	int				i;
	int				j;
	uint32_t		max_indices[3];

	i = -1;
	ft_memset(max_indices, 0, sizeof(max_indices));
	while (++i < (int)obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			max_indices[0] =
				max_indices[0] > obj->triangles[i * 9 + j * 3 + 0] ?
				max_indices[0] : obj->triangles[i * 9 + j * 3 + 0];
			max_indices[1] =
				max_indices[1] > obj->triangles[i * 9 + j * 3 + 1] ?
				max_indices[1] : obj->triangles[i * 9 + j * 3 + 1];
			max_indices[2] =
				max_indices[2] > obj->triangles[i * 9 + j * 3 + 2] ?
				max_indices[2] : obj->triangles[i * 9 + j * 3 + 2];
		}
	}
	return (max_indices[0] == obj->num_vertices && max_indices[1] ==
		obj->num_v_text_coords && max_indices[2] == obj->num_v_normals);
}

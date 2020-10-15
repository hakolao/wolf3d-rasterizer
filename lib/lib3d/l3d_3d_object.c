/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_3d_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:20:51 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/15 17:54:21 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Creates an empty 3d object allocating space for vertices, triangles and
** text coords.
*/

t_3d_object		*l3d_3d_object_create(uint32_t num_vertices,
					uint32_t num_triangles)
{
	t_3d_object	*object;

	error_check(!(object = malloc(sizeof(*object))),
		"Failed to malloc 3d obj");
	error_check(!(object->vertices = malloc(sizeof(t_vertex*) * num_vertices)),
		"Failed to malloc 3d obj vertices");
	ft_memset(object->vertices, 0, sizeof(t_vertex*) * num_vertices);
	error_check(!(object->triangles =
		malloc(sizeof(t_triangle) * num_triangles)),
		"Failed to malloc 3d obj triangles");
	ml_matrix4_id(object->rotation);
	ml_matrix4_id(object->scale);
	ml_vector3_set(object->position, 0, 0, 0);
	object->material.texture = NULL;
	return (object);
}

/*
** Destroys a 3d object.
*/

void			l3d_3d_object_destroy(t_3d_object *object)
{
	int		i;

	free(object->triangles);
	i = -1;
	while (++i < object->num_vertices)
		free(object->vertices[i]);
	free(object->vertices);
	free(object);
	object = NULL;
}

/*
** Just a space saving function to set obj data on 3d object
** (normals, texture coords and position)
*/

void			l3d_3d_object_set_vertex(t_vertex *vertex, t_vec3 pos)
{
	ml_vector3_copy(pos, vertex->pos);
	vertex->color = 0xFFFFFFFF;
}

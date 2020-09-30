/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_3d_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:20:51 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 03:13:48 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Creates an empty 3d object allocating space for vertices, triangles and
** text coords.
*/

t_3d_object		*l3d_3d_object_create(uint32_t num_vertices,
				uint32_t num_triangles, uint32_t num_text_coords)
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
	error_check(!(object->uvs = malloc(sizeof(t_vec2) * num_text_coords)),
		"Failed to malloc 3d obj uvs");
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
** Multiplies each 3d object vertex with t_mat4 transformation matrix,
** eg. translation matrix or rotation. And then updates triangle normals &
** centroids / centers.
*/

void			l3d_3d_object_transform(t_3d_object *obj, t_mat4 transform)
{
	int		i;
	int		j;

	i = -1;
	while (++i < obj->num_vertices)
		ml_matrix4_mul_vec3(transform,
			obj->vertices[i]->pos, obj->vertices[i]->pos);
	j = -1;
	while (++j < obj->num_triangles)
		l3d_triangle_update(&obj->triangles[j]);
}

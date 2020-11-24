/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_3d_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 13:16:03 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/24 13:16:04 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void			l3d_3d_object_debug_print(t_3d_object *obj)
{
	int32_t	i;

	ft_printf("Obj: vertices %d, triangles %d\n", obj->num_vertices, obj->num_triangles);
	if (obj->material)
		ft_printf("Material: width %d, height %d, texture: %p\n",
			obj->material->width, obj->material->height, obj->material->texture);
	else
		ft_printf("Material: NULL");
	ft_printf("Pos: ");
	ml_vector3_print(obj->position);
	ft_printf("Scale:\n");
	ml_matrix4_print(obj->scale);
	ft_printf("Rotation:\n");
	ml_matrix4_print(obj->rotation);
	ft_printf("Vertices:\n");
	i = -1;
	while (++i < obj->num_vertices)
		ml_vector3_print(obj->vertices[i]->pos);
	ft_printf("Triangles:\n");
	i = -1;
	while (++i < obj->num_triangles)
	{
		ft_printf("Vertex indices: %d %d %d\n",
			obj->triangles[i].vtc_indices[0],
			obj->triangles[i].vtc_indices[1],
			obj->triangles[i].vtc_indices[2]);
		ft_printf("triangle center: ");
		ml_vector3_print(obj->triangles[i].center);
	}
}

/*
** Deeply copies 3d object, though the material texture pointer data isn't copied.
*/

t_3d_object		*l3d_3d_object_copy(t_3d_object *src)
{
	t_3d_object	*dst;
	int32_t		i;
	int32_t		j;

	dst = l3d_3d_object_create(src->num_vertices, src->num_triangles);
	if (!dst || !src)
		return (NULL);
	dst->num_vertices = src->num_vertices;
	dst->num_triangles = src->num_triangles;
	ml_matrix4_copy(src->scale, dst->scale);
	ml_vector3_copy(src->position, dst->position);
	ml_matrix4_copy(src->rotation, dst->rotation);
	ft_memcpy(dst->material, src->material, sizeof(t_material));
	i = -1;
	while (++i < src->num_vertices)
		ft_memcpy(dst->vertices[i], src->vertices[i], sizeof(t_vertex));
	i = -1;
	while (++i < src->num_triangles)
	{
		l3d_triangle_set(&dst->triangles[i], (t_vertex*[3]){
			dst->vertices[src->triangles[i].vtc_indices[0]],
			dst->vertices[src->triangles[i].vtc_indices[1]],
			dst->vertices[src->triangles[i].vtc_indices[2]]}, dst);
		j = -1;
		while (++j < 3)
		{
			ml_vector2_copy(src->triangles[i].uvs[j], dst->triangles[i].uvs[j]);
			dst->triangles[i].vtc_indices[j] = src->triangles[i].vtc_indices[j];
			ml_vector2_copy(src->triangles[i].normals[j],
				dst->triangles[i].normals[j]);
		}
	}
	return (dst);
}

/*
** Creates an empty 3d object allocating space for vertices, triangles and
** text coords.
*/

t_3d_object		*l3d_3d_object_create(uint32_t num_vertices,
					uint32_t num_triangles)
{
	t_3d_object	*object;
	int32_t		i;

	error_check(!(object = malloc(sizeof(*object))),
		"Failed to malloc 3d obj");
	error_check(!(object->vertices = malloc(sizeof(t_vertex*) * num_vertices)),
		"Failed to malloc 3d obj vertices");
	i = -1;
	while (++i < (int32_t)num_vertices)
		error_check(!(object->vertices[i] = malloc(sizeof(t_vertex))),
			"Failed to malloc vertex");
	error_check(!(object->triangles =
		malloc(sizeof(t_triangle) * num_triangles)),
		"Failed to malloc 3d obj triangles");
	error_check(!(object->material = malloc(sizeof(t_material))),
		"Failed to malloc 3d obj material");
	ml_matrix4_id(object->rotation);
	ml_matrix4_id(object->scale);
	ml_vector3_set(object->position, 0, 0, 0);
	ft_memset(object->material, 0, sizeof(t_material));
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
	free(object->material);
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

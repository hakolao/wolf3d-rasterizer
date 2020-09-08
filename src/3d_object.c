/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:22:11 by veilo             #+#    #+#             */
/*   Updated: 2020/09/08 16:11:35 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			init_triangle(t_triangle *triangle,
				t_vertex *pos_a, t_vertex *pos_b, t_vertex *pos_c)
{
	t_vec3		e1;
	t_vec3		e2;

	triangle->vtc[0] = pos_a;
	triangle->vtc[1] = pos_b;
	triangle->vtc[2] = pos_c;
	ml_vector3_sub(triangle->vtc[1]->position, triangle->vtc[0]->position, e1);
	ml_vector3_sub(triangle->vtc[2]->position, triangle->vtc[0]->position, e2);
	ml_vector3_cross(e1, e2, triangle->normal);
	ml_vector3_copy(e1, triangle->ab);
	ml_vector3_copy(e2, triangle->ac);
}

void		init_3d_object(t_3d_object *obj, t_vertex *vertices,
			uint32_t vertex_count, t_vec3 origin)
{
	obj->mesh_vertices = vertices;
	obj->mesh_vertex_count = vertex_count;
	ml_vector3_copy(obj->origin, origin);
	ml_vector3_copy(obj->position, origin);
	ml_matrix4_id(obj->transform);
	ml_matrix4_id(obj->mesh_transform);
	// obj->mesh_bound_box; ToDo: Calculate bounding box
	// obj->mesh_triangles; ToDo: Calculate triangles
	// obj->mesh_triangle_count; ToDo: Calculate triangles
}

t_3d_object	*create_3d_object(void)
{
	t_3d_object	*obj;

	error_check(!(obj = malloc(sizeof(*obj))), "Failed to malloc 3d obj");
	return (obj);
}

void		destroy_object(t_3d_object *object)
{
	free(object->mesh_triangles);
	free(object->mesh_vertices);
	free(object);
	object = NULL;
}

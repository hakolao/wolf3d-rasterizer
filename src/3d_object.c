/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:22:11 by veilo             #+#    #+#             */
/*   Updated: 2020/09/14 18:32:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		init_triangle(t_triangle *triangle,
				t_vertex *pos_a, t_vertex *pos_b, t_vertex *pos_c)
{
	ft_memset(triangle->center, 0, sizeof(t_vec3));
	triangle->vtc[0] = pos_a;
	triangle->vtc[1] = pos_b;
	triangle->vtc[2] = pos_c;
	ml_vector3_sub(triangle->vtc[1]->position, triangle->vtc[0]->position,
		triangle->ab);
	ml_vector3_sub(triangle->vtc[2]->position, triangle->vtc[0]->position,
		triangle->ac);
	ml_vector3_cross(triangle->ab, triangle->ac, triangle->normal);
}

static void		set_3d_object_transform_counts(t_obj_result *read_obj,
				t_3d_object *obj)
{
	obj->mesh_triangle_count = read_obj->num_triangles;
	obj->mesh_vertex_count = read_obj->num_vertices;
	obj->mesh_text_coord_count = read_obj->num_v_text_coords;
	ml_vector3_copy((t_vec3){0, 0, 0}, obj->origin);
	ml_vector3_copy((t_vec3){0, 0, 0}, obj->position);
	ml_matrix4_id(obj->transform);
	ml_matrix4_id(obj->mesh_transform);
}

void			init_3d_object(t_obj_result *read_obj, t_3d_object *obj)
{
	int		i;
	int		j;
	int		v_i;
	int		vt_i;
	int		vn_i;

	i = -1;
	while (++i < (int)read_obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			v_i = read_obj->triangles[i][j][0] - 1;
			vt_i = read_obj->triangles[i][j][1] - 1;
			vn_i = read_obj->triangles[i][j][2] - 1;
			ml_vector3_copy(read_obj->v[v_i],
				obj->mesh_vertices[v_i].position);
			ml_vector2_copy(read_obj->vt[vt_i],
				obj->mesh_text_coords[vt_i]);
		}
		init_triangle(&obj->mesh_triangles[i],
			&obj->mesh_vertices[read_obj->triangles[i][0][0] - 1],
			&obj->mesh_vertices[read_obj->triangles[i][1][0] - 1],
			&obj->mesh_vertices[read_obj->triangles[i][2][0] - 1]);
	}
}

t_3d_object		*create_3d_object(t_obj_result *read_obj)
{
	t_3d_object	*obj;

	error_check(!(obj = malloc(sizeof(*obj))), "Failed to malloc 3d obj");
	error_check(!(obj->mesh_vertices =
		malloc(sizeof(t_vertex) * read_obj->num_vertices)),
		"Failed to malloc 3d obj vertices");
	error_check(!(obj->mesh_triangles =
		malloc(sizeof(t_triangle) * read_obj->num_triangles)),
		"Failed to malloc 3d obj triangles");
	error_check(!(obj->mesh_text_coords =
		malloc(sizeof(t_vec2) * read_obj->num_v_text_coords)),
		"Failed to malloc 3d obj text_coords");
	init_3d_object(read_obj, obj);
	set_3d_object_transform_counts(read_obj, obj);
	return (obj);
}

void		destroy_object(t_3d_object *object)
{
	free(object->mesh_triangles);
	free(object->mesh_vertices);
	free(object);
	object = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:22:11 by veilo             #+#    #+#             */
/*   Updated: 2020/09/27 16:25:52 by ohakola          ###   ########.fr       */
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
	l3d_triangle_normal_set(triangle);
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
			if (obj->vertices[v_i] == NULL)
				error_check(!(obj->vertices[v_i] =
					malloc(sizeof(t_vertex))), "Failed to malloc vertex");
			ml_vector3_copy(read_obj->v[v_i],
				obj->vertices[v_i]->position);
		}
		init_triangle(&obj->triangles[i],
			obj->vertices[read_obj->triangles[i][0][0] - 1],
			obj->vertices[read_obj->triangles[i][1][0] - 1],
			obj->vertices[read_obj->triangles[i][2][0] - 1]);
	}
}

t_3d_object		*create_3d_object(t_obj_result *read_obj)
{
	t_3d_object	*obj;

	error_check(!(obj = malloc(sizeof(*obj))), "Failed to malloc 3d obj");
	error_check(!(obj->vertices =
		malloc(sizeof(t_vertex*) * read_obj->num_vertices)),
		"Failed to malloc 3d obj vertices");
	ft_memset(obj->vertices, 0,
		sizeof(t_vertex*) * read_obj->num_vertices);
	error_check(!(obj->triangles =
		malloc(sizeof(t_triangle) * read_obj->num_triangles)),
		"Failed to malloc 3d obj triangles");
	init_3d_object(read_obj, obj);
	obj->num_triangles = read_obj->num_triangles;
	obj->num_vertices = read_obj->num_vertices;
	return (obj);
}

void		transform_3d_object(t_3d_object *obj, t_mat4 transform)
{
	int		i;
	int		j;

	i = -1;
	while (++i < obj->num_vertices)
		ml_matrix4_mul_vec3(transform,
			obj->vertices[i]->position, obj->vertices[i]->position);
	j = -1;
	while (++j < obj->num_triangles)
		l3d_triangle_normal_set(&obj->triangles[j]);
}


void		destroy_object(t_3d_object *object)
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

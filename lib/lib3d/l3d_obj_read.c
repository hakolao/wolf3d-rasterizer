/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_obj_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:27:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/08 13:19:22 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

/*
** Allocates temporary content for obj data struct
*/

void					l3d_obj_content_allocate(t_obj *o)
{
	error_check(!(o->v = malloc(sizeof(t_vec3) * L3D_MAX_VERTICES)),
		"Failed to malloc obj vs");
	error_check(!(o->vt = malloc(sizeof(t_vec2) * L3D_MAX_VERTICES)),
		"Failed to malloc obj v textures");
	error_check(!(o->vn = malloc(sizeof(t_vec3) * L3D_MAX_VERTICES)),
		"Failed to malloc obj v normals");
	error_check(!(o->triangles =
		malloc(sizeof(uint32_t) * 9 * L3D_MAX_TRIANGLES)),
		"Failed to malloc obj triangles");
}

/*
** Frees obj data struct's malloced content
*/

void					l3d_obj_content_free(t_obj *o)
{
	free(o->v);
	free(o->vt);
	free(o->vn);
	free(o->triangles);
}

/*
** Transforms a single obj data struct into t_3d_object.
** @prev_i = previous object's max indices to adjust for multi-object
** read...
*/

static void				obj_to_3d_object(t_obj *read_obj, t_3d_object *obj)
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
			v_i = read_obj->triangles[i * 9 + j * 3 + 0] - 1;
			vt_i = read_obj->triangles[i * 9 + j * 3 + 1] - 1;
			vn_i = read_obj->triangles[i * 9 + j * 3 + 2] - 1;
			error_check(obj->vertices[v_i] == NULL && !(obj->vertices[v_i] =
				malloc(sizeof(t_vertex))), "Failed to malloc vertex");
			l3d_3d_object_set_vertex(obj->vertices[v_i], read_obj->v[v_i],
				read_obj->vt[vt_i], read_obj->vt[vn_i]);
		}
		l3d_triangle_set(&obj->triangles[i],
		obj->vertices[read_obj->triangles[i * 9 + 0 * 3 + 0] - 1],
		obj->vertices[read_obj->triangles[i * 9 + 1 * 3 + 0] - 1],
		obj->vertices[read_obj->triangles[i * 9 + 2 * 3 + 0] - 1]);
	}
}

/*
** Loops through read objects and consolidates them as t_3d_objects to an
** array. Saves the number of objects to inputed num_objects ref.
*/

static t_3d_object		*l3d_3d_object_from_obj(t_obj *obj)
{
	t_3d_object	*l3d_object;

	error_check(!(l3d_object = malloc(sizeof(*l3d_object))),
		"Failed to malloc 3d obj");
	l3d_object = l3d_3d_object_create(obj->num_vertices, obj->num_triangles);
	obj_to_3d_object(obj, l3d_object);
	l3d_object->num_triangles = obj->num_triangles;
	l3d_object->num_vertices = obj->num_vertices;
	l3d_obj_content_free(obj);
	return (l3d_object);
}

/*
** Reads obj file and outputs a list of t_3d_objects saving their number
** into inputted num_objects ref. Invalid obj content will exit, so NULL
** is not returned.
*/

t_3d_object				*l3d_read_obj(const char *filename)
{
	t_file_contents	*obj_file;
	t_obj			obj;

	error_check(!(obj_file = read_file(filename)), "Failed to read file");
	l3d_obj_str_parse((char*)obj_file->buf, &obj);
	if (!l3d_is_valid_obj(&obj))
		return (NULL);
	destroy_file_contents(obj_file);
	return (l3d_3d_object_from_obj(&obj));
}

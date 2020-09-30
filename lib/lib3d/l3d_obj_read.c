/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_obj_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:27:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:08:10 by ohakola          ###   ########.fr       */
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

static void				obj_to_3d_object(t_obj *read_obj, t_3d_object *obj,
						uint32_t prev_i[3])
{
	int		i;
	int		j;
	int		v_i;
	int		vt_i;

	i = -1;
	while (++i < (int)read_obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			v_i = read_obj->triangles[i * 9 + j * 3 + 0] - 1 - prev_i[0];
			vt_i = read_obj->triangles[i * 9 + j * 3 + 1] - 1 - prev_i[1];
			if (obj->vertices[v_i] == NULL)
				error_check(!(obj->vertices[v_i] =
					malloc(sizeof(t_vertex))), "Failed to malloc vertex");
			ml_vector3_copy(read_obj->v[v_i], obj->vertices[v_i]->pos);
			obj->vertices[v_i]->color = 0xFFFFFFFF;
			ml_vector2_copy(read_obj->vt[vt_i], obj->vertices[v_i]->uv);
		}
		l3d_triangle_set(&obj->triangles[i],
		obj->vertices[read_obj->triangles[i * 9 + 0 * 3 + 0] - 1 - prev_i[0]],
		obj->vertices[read_obj->triangles[i * 9 + 1 * 3 + 0] - 1 - prev_i[0]],
		obj->vertices[read_obj->triangles[i * 9 + 2 * 3 + 0] - 1 - prev_i[0]]);
	}
}

/*
** Loops through read objects and consolidates them as t_3d_objects to an
** array. Saves the number of objects to inputed num_objects ref.
*/

static t_3d_object		**l3d_3d_object_from_obj(t_obj_content *obj,
						uint32_t *num_objects)
{
	t_3d_object	**l3d_objects;
	int			i;

	error_check(!(l3d_objects =
		malloc(sizeof(*l3d_objects) * obj->num_objects)),
			"Failed to malloc 3d obj");
	i = -1;
	while (++i < (int)obj->num_objects)
	{
		l3d_objects[i] = l3d_3d_object_create(obj->objects[i].num_vertices,
			obj->objects[i].num_triangles, obj->objects[i].num_v_text_coords);
		obj_to_3d_object(&obj->objects[i], l3d_objects[i],
			i == 0 ? (uint32_t[3]){0, 0, 0} :
			(uint32_t[3]){obj->objects[i - 1].num_vertices,
				obj->objects[i - 1].num_v_text_coords,
				obj->objects[i - 1].num_v_normals});
		l3d_objects[i]->num_triangles = obj->objects[i].num_triangles;
		l3d_objects[i]->num_vertices = obj->objects[i].num_vertices;
		l3d_obj_content_free(&obj->objects[i]);
	}
	*num_objects = obj->num_objects;
	return (l3d_objects);
}

/*
** Reads obj file and outputs a list of t_3d_objects saving their number
** into inputted num_objects ref. Invalid obj content will exit, so NULL
** is not returned.
*/

t_3d_object				**l3d_read_obj(const char *filename,
						uint32_t *num_objects)
{
	t_file_contents	*obj_file;
	t_obj_content	obj_content;

	error_check(!(obj_file = read_file(filename)), "Failed to read file");
	l3d_obj_str_parse((char*)obj_file->buf, &obj_content);
	if (!l3d_is_valid_obj_content(&obj_content))
		return (NULL);
	destroy_file_contents(obj_file);
	return (l3d_3d_object_from_obj(&obj_content, num_objects));
}

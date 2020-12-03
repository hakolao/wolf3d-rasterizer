/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_procedural_gen.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 14:05:45 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/03 17:13:21 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

static void			l3d_plane_set_vertices(t_3d_object *plane)
{
	plane->triangles[0].vtc_indices[0] = 0;
	plane->triangles[0].vtc_indices[1] = 1;
	plane->triangles[0].vtc_indices[2] = 2;
	plane->triangles[1].vtc_indices[0] = 0;
	plane->triangles[1].vtc_indices[1] = 2;
	plane->triangles[1].vtc_indices[1] = 3;
	l3d_triangle_set(&plane->triangles[0], (t_vertex*[3]){
		plane->vertices[0], plane->vertices[1], plane->vertices[2]}, plane);
	l3d_triangle_set(&plane->triangles[1], (t_vertex*[3]){
		plane->vertices[0], plane->vertices[2], plane->vertices[3]}, plane);
}

/*
** Creates a plane facing down.
** ToDo: Face some other direction...
*/

t_3d_object			*l3d_plane_create(t_surface	*texture)
{
	t_3d_object		*plane;

	plane = l3d_3d_object_create(4, 2);
	if (texture != NULL)
		plane->material->texture = texture;
	ml_vector4_copy((t_vec4){-1, 0, -1, 1}, plane->vertices[0]->pos);
	ml_vector4_copy((t_vec4){-1, 0, 1, 1}, plane->vertices[1]->pos);
	ml_vector4_copy((t_vec4){1, 0, 1, 1}, plane->vertices[2]->pos);
	ml_vector4_copy((t_vec4){1, 0, -1, 1}, plane->vertices[3]->pos);
	l3d_plane_set_vertices(plane);
	ml_vector2_copy((t_vec2){0, 1}, plane->triangles[0].uvs[0]);
	ml_vector2_copy((t_vec2){1, 1}, plane->triangles[0].uvs[1]);
	ml_vector2_copy((t_vec2){1, 0}, plane->triangles[0].uvs[2]);
	ml_vector2_copy((t_vec2){0, 1}, plane->triangles[1].uvs[0]);
	ml_vector2_copy((t_vec2){1, 0}, plane->triangles[1].uvs[1]);
	ml_vector2_copy((t_vec2){0, 0}, plane->triangles[1].uvs[2]);
	l3d_object_set_shading_opts(plane, e_shading_ignore_zpass);
	return (plane);
}

/*
** Order of skybox & textures,
** front, left, top, back, right, bottom
*/

void				l3d_skybox_create(t_3d_object *skybox[6],
										t_surface *skybox_textures[6],
										float unit_size)
{
	int32_t		i;
	float		scale;

	scale = unit_size * 10;
	i = -1;
	while (++i < 6)
	{
		skybox[i] = l3d_plane_create(skybox_textures[i]);
		l3d_3d_object_scale(skybox[i],
			scale, scale, scale);
	}
	l3d_3d_object_rotate(skybox[0], 90, 0, 90);
	l3d_3d_object_translate(skybox[0], 0, 0, -scale);
	l3d_3d_object_rotate(skybox[1], -90, -90, 0);
	l3d_3d_object_translate(skybox[1], -scale, 0, 0);
	l3d_3d_object_rotate(skybox[2], 0, -90, 0);
	l3d_3d_object_translate(skybox[2], 0, -scale, 0);
	l3d_3d_object_rotate(skybox[3], 90, -180, 90);
	l3d_3d_object_translate(skybox[3], 0, 0, scale);
	l3d_3d_object_rotate(skybox[4], 0, -90, 90);
	l3d_3d_object_translate(skybox[4], scale, 0, 0);
	l3d_3d_object_translate(skybox[5], 0, scale, 0);
	l3d_3d_object_rotate(skybox[5], 0, -90, 180);
}

/*
** Copies a 3d object / model and places it with given unit scale and position
*/

t_3d_object			*l3d_object_instantiate(t_3d_object *model,
						float unit_size, t_vec3 pos)
{
	t_3d_object	*new_obj;

	new_obj = l3d_3d_object_copy(model);
	l3d_3d_object_scale(new_obj, unit_size, unit_size, unit_size);
	l3d_3d_object_translate(new_obj, pos[0], pos[1], pos[2]);
	return (new_obj);
}

static void			l3d_destroy_temp_object(void *params, size_t size)
{
	t_temp_object	*temp_obj;

	(void)size;
	temp_obj = params;
	l3d_3d_object_destroy(temp_obj->obj);
	free(temp_obj);
	temp_obj = NULL;
}

static t_bool		l3d_temp_objects_expired(t_temp_objects **temp_objects,
						uint32_t current_time,
						uint32_t diff_limit)
{
	t_bool			all_expired;
	t_temp_objects	*tmp;

	tmp = *temp_objects;
	all_expired = true;
	if (!tmp)
		return (all_expired);
	while (tmp)
	{
		if (current_time - ((t_temp_object*)tmp->content)->creation_time <
			diff_limit)
			all_expired = false;
		tmp = tmp->next;
	}
	return (all_expired);
}

void				l3d_temp_objects_destroy_if_expired(t_temp_objects **temp_objects,
														uint32_t current_time,
														uint32_t diff_limit)
{
	if (l3d_temp_objects_expired(temp_objects, current_time, diff_limit))
		l3d_temp_objects_destroy(temp_objects);
}

void				l3d_temp_objects_destroy(t_temp_objects **temp_objects)
{
	ft_lstdel(temp_objects, l3d_destroy_temp_object);
	temp_objects = NULL;
}


void				l3d_temp_objects_add(t_temp_objects **temp_objects,
						t_3d_object *object, uint32_t creation_time)
{
	t_temp_object	*tmp_obj;

	error_check(!(tmp_obj = malloc(sizeof(*tmp_obj))),
		"Failed to malloc temp object");
	tmp_obj->obj = object;
	tmp_obj->creation_time = creation_time;
	if (*temp_objects == NULL)
		*temp_objects = ft_lstnew(tmp_obj, sizeof(*tmp_obj));
	else
		ft_lstadd(temp_objects, ft_lstnew(tmp_obj, sizeof(*tmp_obj)));
}

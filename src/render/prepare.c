/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:46:05 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/06 02:14:11 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_bool	object_inside_viewbox(t_wolf3d *app, t_3d_object *obj)
{
	// int32_t	i;
	// int32_t	is_inside;

	// is_inside = true;
	// i = 0;
	// while (++i < 5)
	// {
	// 	if (ml_vector3_dot(obj->aabb.xyz_min,
	// 		app->active_scene->main_camera->viewplanes[i].normal) > 0 &&
	// 		ml_vector3_dot(obj->aabb.xyz_max,
	// 		app->active_scene->main_camera->viewplanes[i].normal) > 0)
	// 		return (false);
	// }
	(void)app;
	(void)obj;
	return (true);
}

static t_bool	object_behind_player(t_wolf3d *app,
					t_3d_object *obj)
{
	t_vec3		player_to_obj1;
	t_vec3		player_to_obj2;

	ml_vector3_sub(obj->aabb.xyz_min, app->player.pos, player_to_obj1);
	ml_vector3_sub(obj->aabb.xyz_max, app->player.pos, player_to_obj2);
	return (ml_vector3_dot(player_to_obj1, app->player.forward) < 0 &&
		ml_vector3_dot(player_to_obj2, app->player.forward) < 0);
}

static t_bool	object_too_far(t_wolf3d *app, t_3d_object *obj)
{
	float		too_far;
	t_vec3		player_to_obj;

	too_far = app->unit_size * 35;
	ml_vector3_sub(obj->position, app->player.pos,
		player_to_obj);
	if (ml_vector3_mag(player_to_obj) > too_far)
		return (true);
	return (false);
}

/*
** The checks inside this function optimize the amount of triangles
** prepared for rendering.
*/

static void		add_objects_render_triangles(t_wolf3d *app,
					t_tri_vec *render_triangles)
{
	int					i;
	int					j;
	t_triangle			*triangle;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	i = -1;
	while (++i < (int)app->active_scene->num_objects)
	{
		if (!object_inside_viewbox(app, app->active_scene->objects[i]) ||
			object_behind_player(app, app->active_scene->objects[i]) ||
			object_too_far(app, app->active_scene->objects[i]))
			continue ;
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
		{
			triangle = app->active_scene->objects[i]->triangles + j;
			if (triangle_behind_player(app, triangle) ||
				triangle_too_far(app, triangle))
				continue ;
			prepare_render_triangle(app, &r_triangle, triangle, vtc);
			if (is_rendered(app, &r_triangle))
				clip_and_add_to_render_triangles(app,
					render_triangles, &r_triangle);
		}
	}
}

static void		add_skybox_render_triangles(t_wolf3d *app,
					t_tri_vec *render_triangles)
{
	int					i;
	int					j;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	i = -1;
	while (++i < 6 && app->active_scene->skybox[i])
	{
		j = -1;
		while (++j < app->active_scene->skybox[i]->num_triangles)
		{
			prepare_skybox_render_triangle(app, &r_triangle,
				app->active_scene->skybox[i]->triangles + j, vtc);
			if (is_rendered(app, &r_triangle))
				clip_and_add_to_render_triangles(app,
					render_triangles, &r_triangle);
		}
	}
}

void			destroy_render_triangles(t_tri_vec *render_triangles)
{
	int32_t		i;

	i = -1;
	while (++i < (int32_t)render_triangles->size)
	{
		l3d_triangle_destroy(render_triangles->triangles[i], true);
	}
	l3d_triangle_vec_delete(render_triangles);
}

/*
** Prepares triangles for parallel rendering
*/

t_tri_vec		*prepare_render_triangles(t_wolf3d *app)
{
	t_tri_vec			*render_triangles;

	render_triangles =
		l3d_triangle_vec_with_capacity(app->active_scene->num_triangles);
	add_skybox_render_triangles(app, render_triangles);
	add_objects_render_triangles(app, render_triangles);
	return (render_triangles);
}

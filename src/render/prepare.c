/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:46:05 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/06 00:22:53 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
		{
			triangle = app->active_scene->objects[i]->triangles + j;
			if (triangle_too_far(app, triangle) ||
				triangle_behind_player(app, triangle))
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

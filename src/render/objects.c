/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:54:24 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/03 16:33:51 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			rasterize_objects(t_render_work *work)
{
	int					i;
	int					j;
	t_triangle			*triangle;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	i = -1;
	while (++i < (int)work->app->active_scene->num_objects)
	{
		j = -1;
		while (++j < work->app->active_scene->objects[i]->num_triangles)
		{
			triangle = work->app->active_scene->objects[i]->triangles + j;
			set_render_triangle(work->app, &r_triangle, triangle, vtc);
			if (is_rendered(work->app, &r_triangle))
				render_triangle(work->app, work->sub_buffer, &r_triangle);
		}
	}
}

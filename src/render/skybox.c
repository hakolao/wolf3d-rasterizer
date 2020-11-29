/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:46:05 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/26 17:21:04 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** Skybox is rotated, but it is not translated unlike other objects in scene
*/

static void		set_skybox_render_triangle(t_wolf3d *app,
									t_triangle *r_triangle,
									t_triangle *triangle, t_vertex *vtc)
{
	int32_t		i;

	ft_memcpy(r_triangle, triangle, sizeof(t_triangle));
	i = -1;
	while (++i < 3)
	{
		r_triangle->vtc[i] = &vtc[i];
		ml_vector3_copy(triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_rotation,
			r_triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
	}
	l3d_triangle_update(r_triangle);
	r_triangle->clipped = false;
}

void			rasterize_skybox(t_render_work *work)
{
	int					i;
	int					j;
	t_triangle			*triangle;
	t_triangle			r_triangle;
	t_vertex			vtc[3];

	if (!work->app->active_scene->skybox[0])
		return ;
	i = -1;
	while (++i < 6)
	{
		j = -1;
		while (++j < work->app->active_scene->skybox[i]->num_triangles)
		{
			triangle = work->app->active_scene->skybox[i]->triangles + j;
			set_skybox_render_triangle(work->app, &r_triangle, triangle, vtc);
			if (is_rendered(work->app, &r_triangle))
				render_triangle(work->app, work->sub_buffer, &r_triangle,
					rpass_rasterize);
		}
	}
}

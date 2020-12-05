/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 19:06:14 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/06 01:20:04 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_bool			triangle_behind_player(t_wolf3d *app,
					t_triangle *triangle)
{
	t_vec3		player_to_corner[3];
	int32_t		i;

	i = -1;
	while (++i < 3)
		ml_vector3_sub(triangle->vtc[i]->pos, app->player.pos,
			player_to_corner[i]);
	return (ml_vector3_dot(player_to_corner[0], app->player.forward) < 0 &&
		ml_vector3_dot(player_to_corner[1], app->player.forward) < 0 &&
		ml_vector3_dot(player_to_corner[2], app->player.forward) < 0);
}

t_bool			triangle_too_far(t_wolf3d *app, t_triangle *triangle)
{
	float		too_far;
	t_vec3		player_to_corner[3];
	int32_t		i;

	too_far = app->unit_size * 35;
	i = -1;
	while (++i < 3)
		ml_vector3_sub(triangle->vtc[i]->pos, app->player.pos,
			player_to_corner[i]);
	if (ml_vector3_mag(player_to_corner[0]) > too_far &&
		ml_vector3_mag(player_to_corner[1]) > too_far  &&
		ml_vector3_mag(player_to_corner[2]) > too_far )
			return (true);
	return (false);
}

/*
** Apply rotation and translation to render triangle
*/

void			prepare_render_triangle(t_wolf3d *app,
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
		ml_matrix4_mul_vec3(app->player.inv_translation, r_triangle->vtc[i]->pos,
			r_triangle->vtc[i]->pos);
		ml_matrix4_mul_vec3(app->player.inv_rotation,
			r_triangle->vtc[i]->pos, r_triangle->vtc[i]->pos);
	}
	l3d_triangle_update(r_triangle);
	triangle->clipped = false;
}

/*
** Skybox is rotated, but it is not translated unlike other objects in scene
*/

void			prepare_skybox_render_triangle(t_wolf3d *app,
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

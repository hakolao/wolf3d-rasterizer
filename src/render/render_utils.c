/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 17:10:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/05 01:16:08 by ohakola+vei      ###   ########.fr       */
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

	too_far = app->active_scene->map->size * app->unit_size;
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

static void		calculate_2d_points(t_vec2 *points_2d, t_vec3 *hits)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		points_2d[i][0] = hits[i][0];
		points_2d[i][1] = hits[i][1];
	}
}

/*
** Checks if any of triangle vertices are behind near clip distance so the
** perspective projection does not get distorted and nothing "behind" camera
** are drawn. Edit NEAR_CLIP_DIST if needed and you notice that current value
** is not enough.
*/

static t_bool	triangle_behind_camera(t_triangle *triangle, t_camera *camera)
{
	if (triangle->vtc[0]->pos[2] > camera->near_clip &&
		triangle->vtc[1]->pos[2] > camera->near_clip &&
		triangle->vtc[2]->pos[2] > camera->near_clip)
			return (true);
	return (false);
}

static t_bool	is_triangle_facing(t_triangle *triangle, t_vec3 dir)
{
	return (ml_vector3_dot(triangle->normal, dir) <= 0);
}

t_bool			is_rendered(t_wolf3d *app, t_triangle *triangle)
{
	t_vec3 dir;

	if (triangle_behind_camera(triangle, app->active_scene->main_camera))
		return (false);
	ml_vector3_sub(triangle->center, app->active_scene->main_camera->origin, dir);
	if (!is_triangle_facing(triangle, dir))
		return (false);
	return (true);
}

t_bool			screen_intersection(t_wolf3d *app, t_triangle *triangle)
{
	t_ray		rays[3];
	t_vec3		hits[3];
	int			k;

	k = -1;
	while (++k < 3)
	{
		ml_vector3_set(hits[k], 0.0, 0.0, 0.0);
		l3d_ray_set(triangle->vtc[k]->pos, (t_vec3){0, 0, 0}, &rays[k]);
		if (!(l3d_plane_ray_hit(&app->active_scene->main_camera->screen,
			&rays[k], hits[k])))
		{
			ft_printf("Error in screen_intersection: ray from triangle\n"
						"didn't intersect with screen plane or did in too\n"
						"small an angle. Pointo of hit set to default 0.\n"
						"Might happen due to degenerate or badly defined\n"
						"triangles.\n");
			ml_vector3_set_all(hits[k], 0);
		}
	}
	calculate_2d_points(triangle->points_2d, hits);
	return (true);
}

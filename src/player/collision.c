/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 17:17:47 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/12/05 17:58:19 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** https://gamedev.stackexchange.com/questions/49956/
** collision-detection-smooth-wall-sliding-no-bounce-effect
*/

static void		limit_movement_add_by_collision(t_vec3 collision_normal,
					t_vec3 dir_add)
{
	t_vec3		direction_wall_part;

	ml_vector3_mul(collision_normal,
		ml_vector3_dot(dir_add, collision_normal), direction_wall_part);
	ml_vector3_sub(dir_add, direction_wall_part, dir_add);
}

void			collision_limit_player(t_wolf3d *app, t_vec3 add)
{
	int32_t		i;
	t_3d_object	*obj;
	t_player	future_player;
	t_vec3		diff;
	t_hit		*hit;

	ft_memcpy(&future_player, &app->player, sizeof(t_player));
	ml_vector3_add(future_player.pos, add, future_player.pos);
	player_update_aabb(&future_player);
	i = -1;
	while (++i < (int32_t)app->active_scene->num_objects)
	{
		obj = app->active_scene->objects[i];
		ml_vector3_sub(obj->position, future_player.pos, diff);
		if (ml_vector3_mag(diff) > app->unit_size * 3)
			continue ;
		if (l3d_aabb_collides(&obj->aabb, &future_player.aabb))
		{
			l3d_get_aabb_hit_record(&future_player.aabb, &obj->aabb, &hit);
			if (hit != NULL)
			{
				limit_movement_add_by_collision(hit->normal, add);
				free(hit);
			}
		}
	}
}

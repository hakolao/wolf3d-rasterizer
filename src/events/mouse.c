/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 14:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:59 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void					mouse_state_set(t_wolf3d *app)
{
	SDL_PumpEvents();
	app->mouse.state = SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
}

void					determine_closest_triangle_hit(t_hits *hits,
														t_hit **closest)
{
	t_hits	*head;
	t_hit	*hit;

	head = hits;
	*closest = NULL;
	while (head->next)
	{
		hit = (t_hit*)head->content;
		if (*closest == NULL && hit != NULL && hit->t > 0.0 && hit->triangle)
			*closest = hit;
		if (hit != NULL && hit->triangle != NULL && hit->t > 0.0 &&
			hit->t <= (*closest)->t)
			*closest = hit;
		head = head->next;
	}
	if (*closest && !(*closest)->triangle)
		*closest = NULL;
}

void					mouse_state_handle(t_wolf3d *app)
{
	t_ray	ray;
	t_vec3	origin;
	t_vec3	add;
	t_hits	*hits;
	t_hit	*closest_triangle_hit;

	if (app->active_scene->scene_id == scene_id_main_game)
	{
		if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK))
		{
			hits = NULL;
			ml_vector3_mul(app->player.forward, NEAR_CLIP_DIST, add);
			ml_vector3_add(app->player.pos, add, origin);
			l3d_ray_set(app->player.forward, origin, &ray);
			if (l3d_kd_tree_ray_hit(app->active_scene->bullet_tree->root, &ray, &hits))
			{
				determine_closest_triangle_hit(hits, &closest_triangle_hit);
				if (closest_triangle_hit != NULL)
					ft_printf("Hit triangle T: %f\n", closest_triangle_hit->t);
				l3d_delete_hits(&hits);
			}
		}
	}
}

void 					mouse_motion_handle(t_wolf3d *app, SDL_Event event)
{
	if (app->active_scene->scene_id == scene_id_main_game)
	{
		rotate_player_vertical(app, (float)event.motion.yrel * 0.3);
		rotate_player_horizontal(app, -(float)event.motion.xrel * 0.3);
	}
}

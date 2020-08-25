/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 16:10:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		init_scene(t_wolf3d *app)
{
	size_t	z;
	size_t	x;

	error_check((app->scene.objects =
		malloc(sizeof(*app->scene.objects) * 1)) == NULL,
		"Failed to malloc objects");
	app->scene.object_count = 1;
	error_check((app->scene.objects[0].vertices =
		malloc(sizeof(*app->scene.objects[0].vertices) * 100)) == NULL,
		"Failed to malloc objects");
	app->scene.objects[0].vertex_count = 100;
	z = -1;
	while (++z < 10)
	{
		x = -1;
		while (++x < 10)
		{
			ft_memcpy(&app->scene.objects[0].vertices->position,
				&(t_vec4){(float)x, 0, (float)z}, sizeof(t_vec4));
			app->scene.objects[0].vertices->color = 0xff0000ff;
		}
	}
}

void		destroy_scene(t_wolf3d *app)
{
	free(app->scene.objects->vertices);
	free(app->scene.objects);
}
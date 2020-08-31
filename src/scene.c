/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:00:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 17:41:46 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_scene		*new_scene(t_wolf3d *app, t_scenedata *data)
{
	t_scene		*scene;

	scene = (t_scene*)malloc(sizeof(t_scene)); //error checks
	create_object_triangle(scene, app);
	scene->main_camera = new_camera(WIDTH);
	(void)data;
	return (scene);
	//--------------------------------------------------------------------------
	// size_t	y;
	// size_t	x;
	// size_t	i;

	// error_check((app->active_scene.objects =
	// 	malloc(sizeof(*app->active_scene.objects) * 1)) == NULL,
	// 	"Failed to malloc objects");
	// app->active_scene.object_count = 1;
	// error_check((app->active_scene.objects[0].vertices =
	// 	malloc(sizeof(*app->active_scene.objects[0].vertices) * 100)) == NULL,
	// 	"Failed to malloc objects");
	// app->active_scene.objects[0].vertex_count = 100;
	// y = -1;
	// i = 0;
	// while (++y < 10)
	// {
	// 	x = -1;
	// 	while (++x < 10)
	// 	{
	// 		ft_memcpy(&app->active_scene.objects[0].vertices[i].position,
	// 			&(t_vec4){(float)x, (float)y, -5.0, 1.0}, sizeof(t_vec4));
	// 		app->active_scene.objects[0].vertices[i].color = 0xff0000ff;
	// 		i++;
	// 	}
	// }
	//--------------------------------------------------------------------------
}

void		destroy_scene(t_wolf3d *app)
{
	// free(app->active_scene.objects->vertices);
	//free(app->active_scene->objects);
	(void)app;
	return ;
}
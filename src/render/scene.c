/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 17:10:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/13 18:13:43 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	render_active_scene(t_wolf3d *app)
{
	int		i;
	int		j;

	i = -1;
	while (++i < (int)app->active_scene->num_objects)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
			render_triangle(app, &app->active_scene->objects[i]->triangles[j]);
	}
}

void		wolf3d_render(t_wolf3d *app)
{	
	if (app->active_scene->main_camera != NULL)
		render_active_scene(app);
	ui_render(app);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 15:06:43 by veilo             #+#    #+#             */
/*   Updated: 2020/09/02 14:02:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_object		*create_object_triangle(t_scene *scene, t_wolf3d *app)
{
	t_object	*object;

	error_check(!(object = (t_object*)malloc(sizeof(t_object))),
		"Failed to malloc object");
	ml_vec3_set_all(object->origin, 0.0);

	(void)app;
	(void)scene;
	return (NULL);
}


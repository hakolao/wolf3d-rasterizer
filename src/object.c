/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 15:06:43 by veilo             #+#    #+#             */
/*   Updated: 2020/08/31 15:06:43 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_object		*create_object_triangle(t_scene *scene, t_wolf3d *app)
{
	t_object	*object;

	object = (t_object*)malloc(sizeof(t_object)); //implement error check;
	ml_vec3_set_all(object->origin, 0.0);

	(void)app;
	(void)scene;
	return (NULL);
	// object->mesh = create_mesh();
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_ray.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 22:24:15 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/29 22:25:26 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Set ray vectors & pre-calculate dir_inverse for faster bounding box testing.
*/

void						l3d_ray_set(t_vec3 dir, t_vec3 origin, t_ray *ray)
{
	ml_vector3_set(ray->dir, dir[0], dir[1], dir[2]);
	ml_vector3_set(ray->origin, origin[0], origin[1], origin[2]);
	ml_vector3_normalize(ray->dir, ray->dir);
	ml_vector3_set(ray->dir_inv, 1.0 / dir[0], 1.0 / dir[1], 1.0 / dir[2]);
}

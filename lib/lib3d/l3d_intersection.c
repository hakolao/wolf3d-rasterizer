/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:28:46 by veilo             #+#    #+#             */
/*   Updated: 2020/08/25 16:28:46 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
**	Creates a new ray with given parameters.
*/

t_ray			new_ray(t_vec3 origin, t_vec3 direction)
{
	t_ray ray;

	ml_vector3_copy(origin, ray.origin);
	ml_vector3_copy(direction, ray.dir);
	return (ray);
}

/*
**	Calculates the intesection point (if any) of a triangle and a ray.
**	Assumes that both are in the same coordinate space. 
**
**	NOTE: If crashes occur it may be caused by division by zero
**	in this function. It happens if triangle is coplanar with the ray.
**	There is a check in place for zero but it might not be foolproof.
*/

bool			 triangle_intersection(t_triangle *triangle, t_ray ray,
									t_intersection *is)
{
	t_temp_calc temp_calc;
	ml_vector3_normalize(ray.dir, ray.dir);
	ml_vector4_sub(((triangle->vtc)[1])->position, ((triangle->vtc)[0])->position, temp_calc.ab);
	ml_vector4_sub(((triangle->vtc)[2])->position, ((triangle->vtc)[0])->position, temp_calc.ac);
	ml_vector3_sub(ray.origin, ((triangle->vtc)[0])->position, temp_calc.ax);
	temp_calc.det = -1 * ml_vector3_dot(ray.dir, triangle->normal);
	ml_vector3_cross(ray.dir, temp_calc.ax, temp_calc.cross_dir_ax);
	ml_vector3_mul(temp_calc.cross_dir_ax, -1.0, temp_calc.cross_dir_ax);
	ml_vector3_copy(triangle->normal, temp_calc.normal);
	if (temp_calc.det < EPSILON && temp_calc.det > -EPSILON)
		return (false);
	temp_calc.invdet = 1.0 / temp_calc.det;
	is->det = temp_calc.det;
	is->u = ml_vector3_dot(temp_calc.ac, temp_calc.cross_dir_ax)
			* temp_calc.invdet;
	is->v = -1 * ml_vector3_dot(temp_calc.ab, temp_calc.cross_dir_ax)
			* temp_calc.invdet;
	is->t = ml_vector3_dot(temp_calc.ax, temp_calc.normal) * temp_calc.invdet;
	return (is->u >= 0.0 && is->v >= 0.0 && is->t >= 0.0 &&
			(is->u + is->v <= 1.0));
}

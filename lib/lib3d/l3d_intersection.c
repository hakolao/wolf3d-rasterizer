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
**	Calculates the intesection point (if any) of a triangle and a ray.
**	Assumes that both are in the same coordinate space. 
**
**	NOTE: If crashes occur it may be caused by division by zero
**	in this function. It happens if triangle is coplanar with the ray.
**	There is a check in place for zero but it might not be foolproof.
*/

bool			 intersect_triangle(t_triangle triangle, t_ray ray,
									t_intersection *is)
{
	t_is_calc is_calc;
	ml_vector3_normalize(ray.dir, ray.dir);
	ml_vector4_sub(triangle.b.position, triangle.a.position, is_calc.ab);
	ml_vector4_sub(triangle.c.position, triangle.a.position, is_calc.ac);
	ml_vector3_sub(ray.origin, triangle.a.position, is_calc.ax);
	is_calc.det = -1 * ml_vector3_dot(ray.dir, triangle.normal);
	ml_vector3_cross(ray.dir, is_calc.ax, is_calc.cross_dir_ax);
	ml_vector3_mul(is_calc.cross_dir_ax, -1.0, is_calc.cross_dir_ax);
	ml_vector3_copy(triangle.normal, is_calc.normal);
	if (is_calc.det < EPSILON && is_calc.det > -EPSILON)
		return (false);
	is_calc.invdet = 1.0 / is_calc.det;
	is->det = is_calc.det;
	is->u = ml_vector3_dot(is_calc.ac, is_calc.cross_dir_ax)
			* is_calc.invdet;
	is->v = -1 * ml_vector3_dot(is_calc.ab, is_calc.cross_dir_ax)
			* is_calc.invdet;
	is->t = ml_vector3_dot(is_calc.ax, is_calc.normal) * is_calc.invdet;
	return (is->u >= 0.0 && is->v >= 0.0 && is->t >= 0.0 &&
			(is->u + is->v <= 1.0));
}

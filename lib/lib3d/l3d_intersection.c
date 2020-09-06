/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 16:28:46 by veilo             #+#    #+#             */
/*   Updated: 2020/09/01 18:08:54 by ohakola          ###   ########.fr       */
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

t_bool			 triangle_intersection(t_triangle *triangle, t_ray *ray,
									t_intersection *is)
{//initial tests: correct version 81ms per frame
//incorrect version: 57ms per frame
//normalizing a vec3 takes around 17ms: 10 assigments, 3 div, 3 mult, 1 sqrt, 9 comparisons
//triangle size does not affect performance (before boundbox optimizations)
t_vec3 tvec;
t_vec3 qvec;
t_vec3 pvec;
t_temp_calc temp_calc;
ml_vector3_sub(ray->origin, ((triangle->vtc)[0])->position, temp_calc.ax);
temp_calc.det = -1 * ml_vector3_dot(ray->normalized_dir, triangle->normal);	 //det
ml_vector3_cross(ray->normalized_dir, temp_calc.ax, temp_calc.cross_dir_ax); //dao
// ml_vector3_mul(temp_calc.cross_dir_ax, -1.0, temp_calc.cross_dir_ax);//to fix wrong handed crossproduct??
// ml_vector3_copy(triangle->normal, temp_calc.normal);
if (temp_calc.det < EPSILON && temp_calc.det > -EPSILON)
	return (false);
temp_calc.invdet = 1.0 / temp_calc.det;
is->det = temp_calc.det;

ml_vector3_copy(temp_calc.ax, tvec);
ml_vector3_cross(ray->dir, triangle->ac, pvec);
ml_vector3_cross(tvec, triangle->ab, qvec);

// is->u = ml_vector3_dot(triangle->ac, temp_calc.cross_dir_ax)//this gives wrong result
// 		* temp_calc.invdet;
// is->v = -1 * ml_vector3_dot(triangle->ab, temp_calc.cross_dir_ax)
// 		* temp_calc.invdet;
// is->t = ml_vector3_dot(temp_calc.ax, triangle->normal) * temp_calc.invdet;
float invdet = 1.0 / (ml_vector3_dot(triangle->ab, pvec)); //this is the correct way of calculating
is->u = ml_vector3_dot(tvec, pvec) * invdet;
is->v = ml_vector3_dot(ray->dir, qvec) * invdet;
is->t = ml_vector3_dot(triangle->ac, qvec) * invdet;
// temp_calc.det *= -1;
return (temp_calc.det >= EPSILON && is->u >= 0.0 && is->v >= 0.0 && is->t >= 0.0 &&
		(is->u + is->v <= 1.0));
}

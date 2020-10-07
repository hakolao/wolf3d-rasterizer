/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_ray_hit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 18:10:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/06 14:56:29 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Fast branchless bounding box ray hit test.
** Returns true if ray hits the bounding box. If hit occurs, save distance to
** hit record's t variable (to be used in calculating hit vector).
** https://gamedev.stackexchange.com/questions/18436/
** https://tavianator.com/2011/ray_box.html
*/

t_bool			l3d_bounding_box_ray_hit(t_box3d *box, t_ray *ray, t_hit *hit)
{
	float	t[9];

	t[1] = (box->xyz_min[0] - ray->origin[0]) * ray->dir_inv[0];
	t[2] = (box->xyz_max[0] - ray->origin[0]) * ray->dir_inv[0];
	t[3] = (box->xyz_min[1] - ray->origin[1]) * ray->dir_inv[1];
	t[4] = (box->xyz_max[1] - ray->origin[1]) * ray->dir_inv[1];
	t[5] = (box->xyz_min[2] - ray->origin[2]) * ray->dir_inv[2];
	t[6] = (box->xyz_max[2] - ray->origin[2]) * ray->dir_inv[2];
	t[7] = l3d_fmax(l3d_fmax(l3d_fmin(t[1], t[2]), l3d_fmin(t[3], t[4])),
		l3d_fmin(t[5], t[6]));
	t[8] = l3d_fmin(l3d_fmin(l3d_fmax(t[1], t[2]), l3d_fmax(t[3], t[4])),
		l3d_fmax(t[5], t[6]));
	if (t[8] < 0 || t[7] > t[8])
		return (false);
	l3d_bounding_box_hit_record_set(t[7], ray, hit);
	return (true);
}

/*
** A tradeoff with readability & 42 norms. I refuse to set temp structs for just
** this function, because it'll only bloat the header files. In this case
** bundled arrays are better to beautify the api of triangle_ray_hit function.
*/

static t_bool	l3d_determine_triangle_hit(t_vec3 hsq[3],
				t_triangle *triangle, t_ray *ray, t_hit *hit)
{
	float	afuvt[5];

	afuvt[0] = ml_vector3_dot(triangle->ab, hsq[0]);
	if (afuvt[0] > -L3D_EPSILON && afuvt[0] < L3D_EPSILON)
		return (false);
	afuvt[1] = 1.0 / afuvt[0];
	ml_vector3_sub(ray->origin, triangle->vtc[0]->pos, hsq[1]);
	afuvt[2] = afuvt[1] * ml_vector3_dot(hsq[1], hsq[0]);
	if (afuvt[2] < 0.0 || afuvt[2] > 1.0)
		return (false);
	ml_vector3_cross(hsq[1], triangle->ab, hsq[2]);
	afuvt[3] = afuvt[1] * ml_vector3_dot(ray->dir, hsq[2]);
	if (afuvt[3] < 0.0 || afuvt[2] + afuvt[3] > 1.0)
		return (false);
	afuvt[4] = afuvt[1] * ml_vector3_dot(triangle->ac, hsq[2]);
	if (afuvt[4] > L3D_EPSILON)
	{
		l3d_triangle_hit_record_set(afuvt, ray, triangle, hit);
		return (true);
	}
	return (false);
}

/*
** https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
** Returns true if ray hits triangle. A single sidedness test is tested first
** (back-face culling). False is returned if triangle is single sided.
** Hit distance is saved to hit record hit->t.
*/

t_bool			l3d_triangle_ray_hit(t_triangle *triangle, t_ray *ray,
				t_hit *hit)
{
	t_vec3	hsq[3];

	if (ml_vector3_dot(ray->dir, triangle->normal) > 0 &&
		triangle->is_single_sided)
		return (false);
	ml_vector3_cross(ray->dir, triangle->ac, hsq[0]);
	return (l3d_determine_triangle_hit(hsq, triangle, ray, hit));
}

/*
** Loop through all triangles under a kd tree node and save the last triangle's
** hit position to hit record.
** !ToDo: Update this function to save multiple hits to the record, though
** kd tree partition ensures they are rare, they are possible when e.g.
** triangles materials are see-through.
*/

static t_bool	l3d_kd_triangles_hit(t_kd_node *node, t_ray *ray, float t_max,
				t_hit *hit)
{
	t_bool	hit_triangle;
	t_vec3	dir_add;
	int		i;

	hit_triangle = false;
	i = -1;
	while (++i < (int)node->triangles->size)
	{
		if (l3d_triangle_ray_hit(node->triangles->triangles[i], ray, hit))
		{
			hit_triangle = true;
			ml_vector3_mul(ray->dir, hit->t, dir_add);
			ml_vector3_add(ray->origin, dir_add, hit->hit_point);
			ml_vector3_copy(node->triangles->triangles[i]->normal,
				hit->normal);
			t_max = hit->t;
		}
	}
	return (hit_triangle);
}

/*
** Test if outer bounding box is hit on kd tree node and binary search further
** until there are no nodes left nor right. On the final node, test triangle
** hits.
*/

t_bool			l3d_kd_tree_ray_hit(t_kd_node *node, t_ray *ray,
				float t_max, t_hit *hit)
{
	t_bool	hits_right;
	t_bool	hits_left;

	if (l3d_bounding_box_ray_hit(&node->bounding_box, ray, hit))
	{
		if (node->left || node->right)
		{
			hits_left = l3d_kd_tree_ray_hit(node->left, ray, t_max, hit);
			hits_right = l3d_kd_tree_ray_hit(node->right, ray, t_max, hit);
			return (hits_left || hits_right);
		}
		return (l3d_kd_triangles_hit(node, ray, t_max, hit));
	}
	return (false);
}

/*
**	Detects hit between a ray and an infinite plane in 3D. Stores the hit point
**	in a t_vec3.
*/

t_bool			l3d_plane_ray_hit(t_plane *plane, t_ray *ray,
									t_vec3 hit_point)
{
	t_vec3 temp;
	float div;
	float d;

	ml_vector3_sub(plane->origin, ray->origin, temp);
	if (fabs((div = ml_vector3_dot(ray->dir, plane->normal))) > L3D_EPSILON)
	{
		d = (ml_vector3_dot(temp, plane->normal)) / div;
		ml_vector3_mul(ray->dir, d, hit_point);
		ml_vector3_add(hit_point, ray->origin, hit_point);
		return (true);
	}
	return (false);
}
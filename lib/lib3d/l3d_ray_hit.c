/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_ray_hit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 18:10:29 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:35:41 by ohakola          ###   ########.fr       */
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
	hit->t = t[7];
	return (true);
}

/*
** A tradeoff with readability & 42 norms. I refuse to set temp structs for just
** this function, because it'll only bloat the header files. In this case
** bundled arrays are better to beautify the api of triangle_ray_hit function.
*/

static t_bool	l3d_determine_triangle_hit(t_vec3 edges_hsq[5],
				t_triangle *triangle, t_ray *ray, t_hit *hit)
{
	float	afuvt[5];

	afuvt[0] = ml_vector3_dot(edges_hsq[0], edges_hsq[2]);
	if (afuvt[0] > -L3D_EPSILON && afuvt[0] < L3D_EPSILON)
		return (false);
	afuvt[1] = 1.0 / afuvt[0];
	ml_vector3_sub(ray->origin, triangle->vtc[0]->pos, edges_hsq[3]);
	afuvt[2] = afuvt[1] * ml_vector3_dot(edges_hsq[3], edges_hsq[2]);
	if (afuvt[2] < 0.0 || afuvt[2] > 1.0)
		return (false);
	ml_vector3_cross(edges_hsq[3], edges_hsq[0], edges_hsq[4]);
	afuvt[3] = afuvt[1] * ml_vector3_dot(ray->dir, edges_hsq[4]);
	if (afuvt[3] < 0.0 || afuvt[2] + afuvt[3] > 1.0)
		return (false);
	afuvt[4] = afuvt[1] * ml_vector3_dot(edges_hsq[1], edges_hsq[4]);
	if (afuvt[4] > L3D_EPSILON)
	{
		hit->t = afuvt[4];
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
	t_vec3	edges_hsq[5];

	ml_vector3_sub(triangle->vtc[1]->pos, triangle->vtc[0]->pos, edges_hsq[0]);
	ml_vector3_sub(triangle->vtc[2]->pos, triangle->vtc[0]->pos, edges_hsq[1]);
	ml_vector3_cross(ray->dir, edges_hsq[1], edges_hsq[2]);
	if (ml_vector3_dot(ray->dir, triangle->normal) > 0 &&
		triangle->is_single_sided)
		return (false);
	return (l3d_determine_triangle_hit(edges_hsq, triangle, ray, hit));
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

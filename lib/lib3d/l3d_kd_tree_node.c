/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_kd_tree_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 00:10:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:36:28 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

/*
** Destroy's a kd node and its children leaving triangles intact, though the
** triangle vector is deleted.
*/

void				l3d_kd_node_destroy(t_kd_node *root)
{
	t_kd_node	*left;
	t_kd_node	*right;

	if (root)
	{
		l3d_triangle_vec_delete(root->triangles);
		left = root->left;
		right = root->right;
		free(root);
		root = NULL;
		l3d_kd_node_destroy(left);
		l3d_kd_node_destroy(right);
	}
}

/*
** Creates a kd tree node from a triangle vector initializing its values.
*/

t_kd_node			*l3d_kd_node_create(t_tri_vec *triangles)
{
	t_kd_node	*node;

	if (!(node = malloc(sizeof(t_kd_node))))
		return (NULL);
	l3d_bounding_box_set(triangles, &node->bounding_box);
	node->triangles = triangles;
	node->left = NULL;
	node->right = NULL;
	node->axis = l3d_axis_none;
	return (node);
}

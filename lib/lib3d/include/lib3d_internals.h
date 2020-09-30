/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib3d_internals.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 16:07:48 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:11:51 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB3D_INTERNALS_H
# define LIB3D_INTERNALS_H

# include "lib3d.h"

/*
** OBJ reading
*/

void						l3d_read_obj_triangle_line(char **str,
							uint32_t *triangle);
void						l3d_read_obj_vec2_line(char **str, t_vec3 res);
void						l3d_read_obj_vec3_line(char **str, t_vec3 res);
t_bool						l3d_is_valid_obj_content(t_obj_content
							*obj_content);
void						l3d_obj_str_parse(char *str, t_obj_content *result);
void						l3d_obj_content_allocate(t_obj *o);
void						l3d_obj_content_free(t_obj *o);

/*
** Kd Tree
*/

void						l3d_kd_tree_split_triangles(t_tri_vec *triangles,
							uint32_t axis, t_tri_vec *left_tris,
							t_tri_vec *right_tris);
t_kd_node					*l3d_kd_node_create(t_tri_vec *triangles);
void						l3d_kd_node_destroy(t_kd_node *root);

#endif

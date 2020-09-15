/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_obj_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:26:13 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/14 18:15:36 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_bool					is_valid_obj_result(t_obj_result *result)
{
	int				i;
	int				j;
	uint32_t		max_v_index;
	uint32_t		max_vt_index;
	uint32_t		max_vn_index;

	i = -1;
	max_v_index = 0;
	max_vt_index = 0;
	max_vn_index = 0;
	while (++i < (int)result->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			max_v_index = max_v_index > result->triangles[i][j][0] ?
				max_v_index : result->triangles[i][j][0];
			max_vt_index = max_vt_index > result->triangles[i][j][1] ?
				max_vt_index : result->triangles[i][j][1];
			max_vn_index = max_vn_index > result->triangles[i][j][2] ?
				max_vn_index : result->triangles[i][j][2];
		}
	}
	return (max_v_index == result->num_vertices &&
			max_vt_index == result->num_v_text_coords &&
			max_vn_index == result->num_v_normals );
}

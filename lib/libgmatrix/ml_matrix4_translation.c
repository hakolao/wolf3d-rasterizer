/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_matrix4_translation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 23:44:59 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:04 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_matrix4_translation(float to_x, float to_y, float to_z,
			t_mat4 res)
{
	ml_matrix4_id(res);
	res[3][0] = to_x;
	res[3][1] = to_y;
	res[3][2] = to_z;
}

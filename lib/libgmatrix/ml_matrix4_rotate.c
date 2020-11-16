/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_matrix4_rotate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 01:48:44 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:01 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_matrix4_rotate_x(t_mat4 m, float rad, t_mat4 res)
{
	t_mat4	rotation;

	ft_memset(res, 0, sizeof(t_mat4));
	ml_matrix4_rotation_x(rad, rotation);
	ml_matrix4_mul(m, rotation, res);
}

void		ml_matrix4_rotate_y(t_mat4 m, float rad, t_mat4 res)
{
	t_mat4	rotation;

	ft_memset(res, 0, sizeof(t_mat4));
	ml_matrix4_rotation_y(rad, rotation);
	ml_matrix4_mul(m, rotation, res);
}

void		ml_matrix4_rotate_z(t_mat4 m, float rad, t_mat4 res)
{
	t_mat4	rotation;

	ft_memset(res, 0, sizeof(t_mat4));
	ml_matrix4_rotation_z(rad, rotation);
	ml_matrix4_mul(m, rotation, res);
}

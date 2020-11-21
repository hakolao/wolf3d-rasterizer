/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_matrix3_row.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:04:59 by veilo             #+#    #+#             */
/*   Updated: 2020/11/21 19:05:24 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_matrix3_row(t_vec3 row1, t_vec3 row2, t_vec3 row3, t_mat3 res)
{
	ml_vector3_copy(row1, res[0]);
	ml_vector3_copy(row2, res[1]);
	ml_vector3_copy(row3, res[2]);
}

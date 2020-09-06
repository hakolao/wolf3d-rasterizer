/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_set_orientation_base.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 19:30:47 by veilo             #+#    #+#             */
/*   Updated: 2020/08/31 19:30:48 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libgmatrix.h"

/*
**	Sets the input orientation to match the given order of world elemental
**	vectors. For example input 0, 1, 2 gives you the default world orientation
**	but input 1, 0 , 2 switches the x and y axis.
*/

void		ml_set_orientation_base(t_vec3 *vec, int x, int y, int z)
{
	t_vec3	vecx;
	t_vec3	vecy;
	t_vec3	vecz;

	ml_vector3_set(vecx, 1.0, 0.0, 0.0);
	ml_vector3_set(vecy, 0.0, 1.0, 0.0);
	ml_vector3_set(vecz, 0.0, 0.0, 1.0);
	ml_vector3_copy(vecx, vec[x]);
	ml_vector3_copy(vecy, vec[y]);
	ml_vector3_copy(vecz, vec[z]);
}

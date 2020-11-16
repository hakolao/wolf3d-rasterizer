/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_cross.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:02:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:29 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

/*
**	Calculates the cross product between two vectors
**	Gives the result according to right hand rule.
*/

void		ml_vector3_cross(t_vec3 v1, t_vec3 v2, t_vec3 res)
{
	res[0] = v1[1] * v2[2] - v2[1] * v1[2];
	res[1] = v1[2] * v2[0] - v2[2] * v1[0];
	res[2] = v1[0] * v2[1] - v2[0] * v1[1];
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_mul.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:52:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:36 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector3_mul(t_vec3 v1, float k, t_vec3 res)
{
	size_t		i;

	i = -1;
	while (++i < 3)
		res[i] = v1[i] * k;
}

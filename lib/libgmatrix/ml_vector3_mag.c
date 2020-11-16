/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_mag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:56:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:34 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

float		ml_vector3_mag(t_vec3 v)
{
	size_t		i;
	float		res;

	i = -1;
	res = 0;
	while (++i < 3)
		res += v[i] * v[i];
	return (sqrt(res));
}

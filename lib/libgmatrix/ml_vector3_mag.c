/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_mag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:56:05 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 16:57:09 by ohakola          ###   ########.fr       */
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

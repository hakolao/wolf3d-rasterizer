/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_dot.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 17:00:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:31 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

float		ml_vector3_dot(t_vec3 v1, t_vec3 v2)
{
	size_t		i;
	float		res;

	i = -1;
	res = 0;
	while (++i < 3)
		res += v1[i] * v2[i];
	return (res);
}

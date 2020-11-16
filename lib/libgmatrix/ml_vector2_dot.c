/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector2_dot.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 16:59:02 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:14 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

float		ml_vector2_dot(t_vec2 v1, t_vec2 v2)
{
	size_t		i;
	float		res;

	i = -1;
	res = 0;
	while (++i < 2)
		res += v1[i] * v2[i];
	return (res);
}

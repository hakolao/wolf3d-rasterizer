/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector2_mag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:49:32 by veilo             #+#    #+#             */
/*   Updated: 2020/11/16 13:55:16 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

float		ml_vector2_mag(t_vec2 v)
{
	size_t	i;
	float	res;

	i = -1;
	res = 0;
	while (++i < 2)
		res += v[i] * v[i];
	return (sqrt(res));
}

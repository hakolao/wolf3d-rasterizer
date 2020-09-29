/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector2_mag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:49:32 by veilo             #+#    #+#             */
/*   Updated: 2020/09/30 01:49:58 by ohakola          ###   ########.fr       */
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

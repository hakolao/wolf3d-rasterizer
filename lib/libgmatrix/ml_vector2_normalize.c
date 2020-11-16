/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector2_normalize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 13:11:34 by veilo             #+#    #+#             */
/*   Updated: 2020/11/16 13:55:19 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void	ml_vector2_normalize(t_vec2 v, t_vec2 res)
{
	size_t	i;
	float	mag;

	i = -1;
	mag = ml_vector2_mag(v);
	while (++i < 2)
	{
		if (mag > 0)
			res[i] = v[i] / mag;
	}
}

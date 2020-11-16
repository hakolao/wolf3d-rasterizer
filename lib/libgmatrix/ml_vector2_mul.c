/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector2_mul.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 16:47:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:17 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector2_mul(t_vec2 v1, float k, t_vec2 res)
{
	size_t		i;

	i = -1;
	while (++i < 2)
		res[i] = v1[i] * k;
}

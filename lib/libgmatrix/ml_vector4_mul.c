/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector4_mul.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:52:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:48 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector4_mul(t_vec4 v1, float k, t_vec4 res)
{
	size_t		i;

	ft_memset(res, 0, sizeof(t_vec4));
	i = -1;
	while (++i < 4)
		res[i] = v1[i] * k;
}

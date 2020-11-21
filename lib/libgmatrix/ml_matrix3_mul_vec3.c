/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_matrix3_mul_vec3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 19:10:12 by veilo             #+#    #+#             */
/*   Updated: 2020/11/21 19:10:13 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_matrix3_mul_vec3(t_mat3 m, t_vec3 v, t_vec3 res)
{
	size_t r;
	size_t c;

	ft_memset(res, 0, sizeof(t_vec3));
	c = -1;
	while (++c < 3)
	{
		r = -1;
		while (++r < 3)
			res[c] += m[r][c] * v[r];
	}
}
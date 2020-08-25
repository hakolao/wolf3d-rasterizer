/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector4_mul.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:52:07 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/25 16:56:29 by ohakola          ###   ########.fr       */
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

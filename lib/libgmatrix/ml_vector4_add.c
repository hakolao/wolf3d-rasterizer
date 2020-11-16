/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector4_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:49:09 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:43 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector4_add(t_vec4 v1, t_vec4 v2, t_vec4 res)
{
	size_t		i;

	i = -1;
	while (++i < 4)
		res[i] = v1[i] + v2[i];
}

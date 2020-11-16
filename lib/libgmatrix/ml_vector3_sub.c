/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_sub.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:51:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:40 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector3_sub(t_vec3 v1, t_vec3 v2, t_vec3 res)
{
	size_t		i;

	i = -1;
	while (++i < 3)
		res[i] = v1[i] - v2[i];
}

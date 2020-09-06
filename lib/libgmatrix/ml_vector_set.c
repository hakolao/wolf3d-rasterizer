/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_set.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 17:52:39 by veilo             #+#    #+#             */
/*   Updated: 2020/08/31 17:52:40 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector3_set(t_vec3 vec, float v0, float v1, float v2)
{
	vec[0] = v0;
	vec[1] = v1;
	vec[2] = v2;
}

void		ml_vector3_set_all(t_vec3 vec, float val)
{
	int		i;

	i = -1;
	while (i++ < 3)
	{
		vec[i] = val;
	}
}

void		ml_vector4_set_all(t_vec4 vec, float val)
{
	int		i;

	i = -1;
	while (i++ < 4)
	{
		vec[i] = val;
	}
}
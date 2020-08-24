/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector4_normalize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:53:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 19:48:17 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector4_normalize(t_vec4 v, t_vec4 res)
{
	size_t		i;
	float		mag;

	i = -1;
	mag = ml_vector4_mag(v);
	while (++i < 4)
		res[i] = v[i] / mag;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector2_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:52:23 by veilo             #+#    #+#             */
/*   Updated: 2020/11/16 13:55:13 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void	ml_vector2_add(t_vec2 v1, t_vec2 v2, t_vec2 res)
{
	size_t i;

	i = -1;
	while (++i < 2)
		res[i] = v1[i] + v2[i];
}

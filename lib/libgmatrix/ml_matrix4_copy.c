/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_matrix4_copy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 23:52:26 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/08 23:54:12 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

/*
**	Copies the elements of one matrix to another for t_mat4
*/

void	ml_matrix4_copy(t_mat4 src, t_mat4 dest)
{
	int i;

	i = 0;
	while (i < 4)
	{
		ml_vector4_copy(src[i], dest[i]);
		i++;
	}
}

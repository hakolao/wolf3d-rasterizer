/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ml_vector3_forward.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 16:57:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:55:33 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"

void		ml_vector3_forward(t_vec3 eye, t_vec3 target, t_vec3 res)
{
	t_vec3	forward_unnormalized;

	ml_vector3_sub(target, eye, forward_unnormalized);
	ml_vector3_normalize(forward_unnormalized, res);
}

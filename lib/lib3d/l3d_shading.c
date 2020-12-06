/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_shading.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 17:27:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/06 17:27:41 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

void			l3d_object_set_shading_opts(t_3d_object *obj,
					t_shading_opts opts)
{
	if (obj->material != NULL)
		obj->material->shading_opts = opts;
}

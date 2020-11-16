/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_fminmax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:01:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/16 13:53:14 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Returns min of two float values
*/

float	l3d_fmin(float n1, float n2)
{
	return (n1 < n2 ? n1 : n2);
}

/*
** Returns max of two float values
*/

float	l3d_fmax(float n1, float n2)
{
	return (n1 > n2 ? n1 : n2);
}

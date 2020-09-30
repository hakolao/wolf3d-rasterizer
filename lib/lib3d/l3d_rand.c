/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_rand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 16:20:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 00:39:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

/*
** Just a simple random number between 0.0 and 1.0
*/

double			l3d_rand_d(void)
{
	return ((double)rand() / RAND_MAX);
}

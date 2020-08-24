/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:18:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:18:59 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void				error_check(int test, const char *message)
{
	if (test)
	{
		ft_printf("%s\n", message);
		exit(EXIT_FAILURE);
	}
}
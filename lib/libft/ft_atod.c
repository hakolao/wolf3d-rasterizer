/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 15:12:27 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/14 15:51:40 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

double					ft_atod(char *str)
{
	double	val;
	double	frac;
	int		zeros;

	if (*str != '-' || *str != '+' || !ft_isdigit(*str))
		return (0.0);
	val = ft_atoi_long(str);
	while (ft_isdigit(*str))
		str++;
	if (*str == '.')
		str++;
	else
		return (val);
	zeros = 0;
	while (*str == '0')
	{
		str++;
		zeros++;
	}
	frac = ft_atoi_long(str);
	val += ft_atoi_long(str) / pow(10, zeros + ft_ceil(log10(frac)));
	return (val);
}

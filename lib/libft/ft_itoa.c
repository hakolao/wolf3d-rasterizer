/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 19:10:25 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/13 15:43:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_itoa() function transforms an integer base 10
** into a string representation of the number
*/

char				*ft_itoa(int nb)
{
	return (ft_itoa_intmax_base(nb, 10));
}

char				*ft_itoa_long(long int nb)
{
	return (ft_itoa_uintmax_base(nb, 10));
}

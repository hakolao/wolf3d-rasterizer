/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_long_base.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:31:31 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/13 15:42:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char				*ft_itoa_long_base(long int nb, long int base)
{
	return (ft_itoa_intmax_base(nb, base));
}

char				*ft_itoa_long_u_base(unsigned long int nb,
					unsigned long int base)
{
	return (ft_itoa_uintmax_base(nb, base));
}

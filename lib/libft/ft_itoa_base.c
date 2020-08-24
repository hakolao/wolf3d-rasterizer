/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 15:21:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/13 15:42:47 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t				get_num_len(long long unsigned int nb,
					long long unsigned int base)
{
	size_t	i;

	if (nb == FALSE)
		return (1);
	i = 0;
	while (nb != 0)
	{
		nb = nb / base;
		i++;
	}
	return (i);
}

char				*ft_itoa_base(int nb, int base)
{
	return (ft_itoa_intmax_base(nb, base));
}

char				*ft_itoa_u_base(unsigned int nb, unsigned int base)
{
	return (ft_itoa_uintmax_base(nb, base));
}

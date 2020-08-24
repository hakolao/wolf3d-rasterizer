/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_longl_base.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:35:06 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/13 15:43:03 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char				*ft_itoa_longl_base(long long int nb, long long int base)
{
	return (ft_itoa_intmax_base(nb, base));
}

char				*ft_itoa_longl_u_base(unsigned long long int nb,
					unsigned long long int base)
{
	return (ft_itoa_uintmax_base(nb, base));
}

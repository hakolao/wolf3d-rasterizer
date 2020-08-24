/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_intmax_base.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:35:39 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/13 15:40:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char				*ft_itoa_intmax_base(intmax_t nb, intmax_t base)
{
	int		i;
	char	*arr;
	int		sign;
	char	*bases;

	sign = nb < 0 ? -1 : 1;
	if (base > 16 || base < 1)
		return (NULL);
	bases = "0123456789abcdef";
	if (!(arr = ft_strnew(get_num_len(nb, base) + (sign < 0 ? 1 : 0))))
		return (NULL);
	if (nb == FALSE)
		arr[0] = '0';
	i = 0;
	while (nb != 0)
	{
		arr[i++] = bases[sign * (nb % base)];
		nb = nb / base;
	}
	if (sign == -1)
		arr[i] = '-';
	ft_strrev(arr);
	return (arr);
}

char				*ft_itoa_uintmax_base(uintmax_t nb, uintmax_t base)
{
	int		i;
	char	*arr;
	char	*bases;

	if (base > 16 || base < 1)
		return (NULL);
	bases = "0123456789abcdef";
	if (!(arr = ft_strnew(get_num_len(nb, base))))
		return (NULL);
	if (nb == FALSE)
		arr[0] = '0';
	i = 0;
	while (nb != 0)
	{
		arr[i++] = bases[nb % base];
		nb = nb / base;
	}
	ft_strrev(arr);
	return (arr);
}

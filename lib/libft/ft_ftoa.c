/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 13:19:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/15 21:27:04 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*add_zeros(long long int fpart, int precision, char *str)
{
	int		i;
	int		j;
	int		len;
	int		zeros;
	char	*res;

	len = get_num_len(fpart, 10);
	zeros = precision - len;
	if (zeros <= 0)
		return (str);
	if (!(res = ft_strnew(ft_strlen(str) + zeros)))
		return (NULL);
	res = ft_strcpy(res, str);
	i = ft_strlen(str);
	j = 0;
	while (j < zeros)
	{
		res[i] = '0';
		j++;
		i++;
	}
	ft_strdel(&str);
	return (res);
}

static char		*handle_negative_zero(long double nb, char *str)
{
	char	*res;
	int		len;

	if (nb < 0 && nb > -1)
	{
		len = ft_strlen(str);
		if (!(res = ft_strnew(len + 1)))
			return (NULL);
		res = ft_strcpy(res, str);
		ft_strrev(res);
		res[len] = '-';
		ft_strrev(res);
		ft_strdel(&str);
		return (res);
	}
	return (str);
}

char			*ft_ftoa(long double nb, int precision)
{
	long long int	ipart;
	long double		fpart;
	char			*result;
	char			*tmp1;
	char			*tmp2;

	ipart = (long long int)(nb +
		(nb >= 0 ? 1 : -1) * 0.5 / ft_powl(10, precision));
	fpart = ft_abs_long_double(nb) - ft_abs_long_double((long double)ipart);
	if (!(result = ft_itoa_intmax_base(ipart, 10)) ||
		!(result = handle_negative_zero(nb, result)))
		return (NULL);
	if (precision <= 0)
		return (result);
	if (!(tmp1 = ft_strjoin(result, ".")))
		return (NULL);
	ft_strdel(&result);
	fpart = fpart * ft_powl(10, precision);
	if (!(tmp1 = add_zeros(fpart + 0.5, precision, tmp1)) ||
		!(tmp2 = ft_itoa_intmax_base(fpart + 0.5, 10)) ||
		!(result = ft_strjoin(tmp1, tmp2)))
		return (NULL);
	ft_strdel(&tmp1);
	ft_strdel(&tmp2);
	return (result);
}

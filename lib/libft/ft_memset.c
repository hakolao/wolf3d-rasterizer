/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:47:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/19 15:39:41 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

static void		set_by_3(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		dest[i + 2] = (unsigned char)c;
		i += 3;
	}
}

static void		set_by_2(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		i += 2;
	}
}

/*
** The ft_memset() function writes len bytes of value c
** (converted to an unsigned char) to the string b.
*/

void			*ft_memset(void *b, int c, size_t n)
{
	unsigned char *cpy;

	cpy = b;
	if (b == NULL)
		return (NULL);
	if (n % 8 == 0)
		set_by_8(cpy, c, n);
	else if (n % 7 == 0)
		set_by_7(cpy, c, n);
	else if (n % 6 == 0)
		set_by_6(cpy, c, n);
	else if (n % 5 == 0)
		set_by_5(cpy, c, n);
	else if (n % 4 == 0)
		set_by_4(cpy, c, n);
	else if (n % 3 == 0)
		set_by_3(cpy, c, n);
	else if (n % 2 == 0)
		set_by_2(cpy, c, n);
	else
		while (n--)
			*cpy++ = (unsigned char)c;
	return (b);
}

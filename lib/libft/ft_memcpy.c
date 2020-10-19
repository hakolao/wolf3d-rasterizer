/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:33:56 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/19 15:31:29 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

static void		copy_by_3(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		destcpy[i + 2] = srccpy[i + 2];
		i += 3;
	}
}

static void		copy_by_2(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		i += 2;
	}
}

/*
** The ft_memcpy() function copies n bytes from memory area src to memory
** area dst.
** If dst and src overlap, behavior is undefined.  Applications in
** which dst and src might overlap should use memmove(3) instead.
** This version has been optimized to copy more at once.
*/

void			*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*destcpy;
	const char	*srccpy;

	if (src == NULL && dst == NULL)
		return (NULL);
	destcpy = dst;
	srccpy = src;
	if (n % 8 == 0)
		copy_by_8(destcpy, srccpy, n);
	else if (n % 7 == 0)
		copy_by_7(destcpy, srccpy, n);
	else if (n % 6 == 0)
		copy_by_6(destcpy, srccpy, n);
	else if (n % 5 == 0)
		copy_by_5(destcpy, srccpy, n);
	else if (n % 4 == 0)
		copy_by_4(destcpy, srccpy, n);
	else if (n % 3 == 0)
		copy_by_3(destcpy, srccpy, n);
	else if (n % 2 == 0)
		copy_by_2(destcpy, srccpy, n);
	else
		while (n--)
			*destcpy++ = *srccpy++;
	return (dst);
}

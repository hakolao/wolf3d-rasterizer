/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 14:12:14 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/19 15:38:42 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

void		set_by_8(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		dest[i + 2] = (unsigned char)c;
		dest[i + 3] = (unsigned char)c;
		dest[i + 4] = (unsigned char)c;
		dest[i + 5] = (unsigned char)c;
		dest[i + 6] = (unsigned char)c;
		dest[i + 7] = (unsigned char)c;
		i += 8;
	}
}

void		set_by_7(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		dest[i + 2] = (unsigned char)c;
		dest[i + 3] = (unsigned char)c;
		dest[i + 4] = (unsigned char)c;
		dest[i + 5] = (unsigned char)c;
		dest[i + 6] = (unsigned char)c;
		i += 7;
	}
}

void		set_by_6(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		dest[i + 2] = (unsigned char)c;
		dest[i + 3] = (unsigned char)c;
		dest[i + 4] = (unsigned char)c;
		dest[i + 5] = (unsigned char)c;
		i += 6;
	}
}

void		set_by_5(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		dest[i + 2] = (unsigned char)c;
		dest[i + 3] = (unsigned char)c;
		dest[i + 4] = (unsigned char)c;
		i += 5;
	}
}

void		set_by_4(unsigned char *dest, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		dest[i + 1] = (unsigned char)c;
		dest[i + 2] = (unsigned char)c;
		dest[i + 3] = (unsigned char)c;
		i += 4;
	}
}

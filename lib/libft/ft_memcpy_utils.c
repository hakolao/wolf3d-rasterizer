/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 14:12:14 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/19 14:19:14 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

void		copy_by_8(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		destcpy[i + 2] = srccpy[i + 2];
		destcpy[i + 3] = srccpy[i + 3];
		destcpy[i + 4] = srccpy[i + 4];
		destcpy[i + 5] = srccpy[i + 5];
		destcpy[i + 6] = srccpy[i + 6];
		destcpy[i + 7] = srccpy[i + 7];
		i += 8;
	}
}

void		copy_by_7(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		destcpy[i + 2] = srccpy[i + 2];
		destcpy[i + 3] = srccpy[i + 3];
		destcpy[i + 4] = srccpy[i + 4];
		destcpy[i + 5] = srccpy[i + 5];
		destcpy[i + 6] = srccpy[i + 6];
		i += 7;
	}
}

void		copy_by_6(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		destcpy[i + 2] = srccpy[i + 2];
		destcpy[i + 3] = srccpy[i + 3];
		destcpy[i + 4] = srccpy[i + 4];
		destcpy[i + 5] = srccpy[i + 5];
		i += 6;
	}
}

void		copy_by_5(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		destcpy[i + 2] = srccpy[i + 2];
		destcpy[i + 3] = srccpy[i + 3];
		destcpy[i + 4] = srccpy[i + 4];
		i += 5;
	}
}

void		copy_by_4(char *destcpy, const char *srccpy, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n)
	{
		destcpy[i] = srccpy[i];
		destcpy[i + 1] = srccpy[i + 1];
		destcpy[i + 2] = srccpy[i + 2];
		destcpy[i + 3] = srccpy[i + 3];
		i += 4;
	}
}

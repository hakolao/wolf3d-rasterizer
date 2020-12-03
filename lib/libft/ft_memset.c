/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:47:36 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/03 23:52:15 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_utils.h"

/*
** The ft_memset() function writes len bytes of value c
** (converted to an unsigned char) to the string b.
*/

void			*ft_memset(void *b, int c, size_t n)
{
	unsigned char *cpy;

	cpy = b;
	while (n--)
		*cpy++ = (unsigned char)c;
	return (b);
}

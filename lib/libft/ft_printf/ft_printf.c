/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:00:25 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/17 00:10:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int				init_printf(t_printf *data, int fd)
{
	data->fd = fd;
	data->len = 0;
	if (!(data->buffer = ft_strnew(0)))
		return (FALSE);
	return (TRUE);
}

int						ft_dprintf(int fd, const char *format, ...)
{
	t_printf	data;

	if (!(init_printf(&data, fd)))
		return (FALSE);
	va_start(data.variables, format);
	if (!parse_input(&data, (char*)format))
		return (FALSE);
	va_end(data.variables);
	write(data.fd, data.buffer, data.len);
	ft_strdel(&data.buffer);
	return (data.len);
}

int						ft_sprintf(char *str, const char *format, ...)
{
	t_printf	data;

	if (!(init_printf(&data, 1)))
		return (FALSE);
	va_start(data.variables, format);
	if (!parse_input(&data, (char*)format))
		return (FALSE);
	va_end(data.variables);
	ft_strncpy(str, data.buffer, data.len);
	ft_strdel(&data.buffer);
	return (data.len);
}

int						ft_printf(const char *format, ...)
{
	t_printf	data;

	if (!(init_printf(&data, 1)))
		return (FALSE);
	va_start(data.variables, format);
	if (!parse_input(&data, (char*)format))
		return (FALSE);
	va_end(data.variables);
	write(data.fd, data.buffer, data.len);
	ft_strdel(&data.buffer);
	return (data.len);
}

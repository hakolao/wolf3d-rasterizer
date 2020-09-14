/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 15:30:06 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/14 15:52:18 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_file_contents		*read_while(int fd)
{
	char			buf[FILE_READ_BUF + 1];
	int				i;
	int				ret;
	void			*tmp;
	t_file_contents	*contents;

	i = 0;
	if (!(contents = malloc(sizeof(*contents))))
		return (NULL);
	while ((ret = read(fd, buf, FILE_READ_BUF)) > 0)
	{
		if (i == 0 && (contents->buf = malloc(ret)))
			ft_memcpy(contents->buf, buf, ret);
		else if (i == 0)
			return (NULL);
		else
		{
			tmp = contents->buf;
			if (!((contents->buf = malloc(i * FILE_READ_BUF + ret))))
				return (NULL);
			ft_memcpy(contents->buf, tmp, (i * FILE_READ_BUF));
			ft_memcpy(contents->buf + i * FILE_READ_BUF, buf, ret);
			free(tmp);
		}
		contents->size = i * FILE_READ_BUF + ret;
		i++;
	}
	ft_memset(contents->buf + contents->size, 0, 1);
	return (contents);
}

t_file_contents				*read_file(const char *filename)
{
	t_file_contents	*contents;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) == -1 &&
		ft_dprintf(2, "Failed to open file %s\n", filename))
		return (NULL);
	contents = read_while(fd);
	if ((fd = close(fd)) == -1 &&
		ft_dprintf(2, "Failed to close file %s\n", filename))
		return (NULL);
	return (contents);
}
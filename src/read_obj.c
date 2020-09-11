/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 17:03:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/11 18:40:50 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_file_contents		*read_while(int fd)
{
	char			buf[FILE_READ_BUF + 1];
	int				i;
	int				ret;
	void			*tmp;
	t_file_contents	*contents;

	i = 0;
	error_check(!(contents = malloc(sizeof(*contents))), "Failed to malloc!");
	while ((ret = read(fd, buf, FILE_READ_BUF)) > 0)
	{
		if (i == 0 && (contents->buf = malloc(ret)))
			ft_memcpy(contents->buf, buf, ret);
		else if (i == 0)
			error_check(true, "Failed to malloc!");
		else
		{
			tmp = contents->buf;
			error_check(!(contents->buf = malloc((i * FILE_READ_BUF + ret))),
				"Failed  to malloc!");
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

t_file_contents			*read_file(const char *filename)
{
	t_file_contents	*contents;
	int				fd;

	if ((fd = open(filename, O_RDONLY)) == -1 &&
		ft_dprintf(2, "Failed to open file %s\n", filename))
		error_check(true, "");
	contents = read_while(fd);
	if ((fd = close(fd)) == -1 &&
		ft_dprintf(2, "Failed to close file %s\n", filename))
		error_check(true, "");
	return (contents);
}

double					ft_atod(char *str)
{
	float	val;
	float	frac;

	if (*str != '-' || *str != '+' || !ft_isdigit(*str))
		return (0.0);
	val = ft_atoi(str);
	while (ft_isdigit(*str) || *str == '.')
		str++;
	frac = ft_atoi(str);
	val += ft_atoi(str) / pow(10, ft_ceil(log10(frac)));
	return (val);
}

void					read_obj_vec3x(char *str, t_vec3 res)
{
	res[0] = ft_atod(str);
	while (*str != ' ');
		str++;
	res[1] = ft_atod(str);
	while (*str != ' ');
		str++;
	res[2] = ft_atod(str);
}

void					read_obj_vec2(char *str, t_vec3 res)
{
	res[0] = ft_atod(str);
	while (*str != ' ');
		str++;
	res[1] = ft_atod(str);
}

void					read_obj_triangle(char *str, uint32_t triangle[3][3])
{
	
}

void					read_obj(const char *filename)
{
	t_file_contents	*obj_file;
	char			*obj_str;
	t_vec3			v[256];
	t_vec2			vt[256];
	t_vec2			vn[256];
	uint32_t		triangles[256][3][3];
	uint32_t		v_index;
	uint32_t		vt_index;
	uint32_t		vn_index;
	uint32_t		tr_index;
	float			val;
	float			frac;

	obj_file = read_file(filename);
	obj_str = (char*)obj_file->buf;
	v_index = 0;
	vt_index = 0;
	vn_index = 0;
	tr_index = 0;
	while (*obj_str)
	{
		// Line is a vertex coord
		if (*obj_str == 'v' && *(obj_str + 1) == ' ')
		{
			read_obj_vec3(obj_str + 1, v[v_index++]);
			while (*obj_str != '\n')
				obj_str++;
		}
		// Line is a texture coord	
		else if (*obj_str == 'v' && *obj_str == 't' && *(obj_str + 2) == ' ')
		{
			read_obj_vec2(obj_str + 2, vt[vt_index]);
			while (*obj_str != '\n')
				obj_str++;
		}
		// Line is a normal coord	
		else if (*obj_str == 'v' && *obj_str == 'n' && *(obj_str + 2) == ' ')
		{
			read_obj_vec2(obj_str + 2, vn[vn_index]);
			while (*obj_str != '\n')
				obj_str++;
		}
		// Line is a face (triangle)
		else if (*obj_str == 'f' && *(obj_str + 1) == ' ')
		{
			read_obj_triangle(obj_str + 1, triangles[tr_index]);
			while (*obj_str != '\n')
				obj_str++;
		}
		else
			obj_str++;
	}
}
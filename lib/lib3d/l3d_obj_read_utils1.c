/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_obj_read_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:35:00 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/07 22:26:22 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

/*
** Parses a vec 3 line from string ref and scrolls the str
** forward along the way.
*/

void					l3d_read_obj_vec3_line(char **str, t_vec3 res)
{
	char	*tmp;

	tmp = *str;
	while (*tmp &&
		(ft_isdigit(*tmp) || *tmp == ' ' || *tmp == '-' || *tmp == '.'))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in vec3 data (v)");
	res[0] = ft_atod(*str);
	ft_scroll_over(str, ' ');
	res[1] = ft_atod(*str);
	ft_scroll_over(str, ' ');
	res[2] = ft_atod(*str);
	ft_scroll_over(str, '\n');
}

/*
** Parses a vec 2 line from string ref and scrolls the str
** forward along the way.
*/

void					l3d_read_obj_vec2_line(char **str, t_vec3 res)
{
	char	*tmp;

	tmp = *str;
	while (*tmp &&
		(ft_isdigit(*tmp) || *tmp == ' ' || *tmp == '-' || *tmp == '.'))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in vec2 data (vn || vt)");
	res[0] = ft_atod(*str);
	ft_scroll_over(str, ' ');
	res[1] = ft_atod(*str);
	ft_scroll_over(str, '\n');
}

/*
** Parses a triangulated face line from string ref and scrolls the str
** forward along the way.
*/

void					l3d_read_obj_triangle_line(char **str,
						uint32_t *triangle)
{
	char	*tmp;
	int		i;

	tmp = *str;
	while (*tmp && (*tmp == '/' || ft_isdigit(*tmp) || *tmp == ' '))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in triangle data");
	i = -1;
	while (++i < 9)
	{
		*(triangle + i) = ft_atoi(*str);
		if (i == 2 || i == 5)
			ft_scroll_over(str, ' ');
		else if (i < 8)
			ft_scroll_over(str, '/');
	}
	tmp = *str;
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid triangle, read_obj expect faces to be "
			"triangles e.g. (1/1/1/ 2/2/2 3/3/3\n");
	ft_scroll_over(str, '\n');
}

/*
** Parses a single obj from obj str ref and scroll the string to
*/

void					l3d_obj_parse_single_obj(char **str, t_obj *obj)
{
	l3d_obj_content_allocate(obj);
	while (!(**str == 'v' && *(*str + 1) == ' '))
		(*str)++;
	while (**str)
	{
		if (**str == 'v' && *(*str + 1) == ' ' &&
			(*str += 2))
			l3d_read_obj_vec3_line(str, obj->v[obj->num_vertices++]);
		else if (**str == 'v' && *(*str + 1) == 't' && *(*str + 2) == ' ' &&
			(*str += 3))
			l3d_read_obj_vec2_line(str, obj->vt[obj->num_v_text_coords++]);
		else if (**str == 'v' && *(*str + 1) == 'n' && *(*str + 2) == ' ' &&
			(*str += 3))
			l3d_read_obj_vec3_line(str, obj->vn[obj->num_v_normals++]);
		else if (**str == 'f' && *(*str + 1) == ' ' &&
			(*str += 2))
		{
			l3d_read_obj_triangle_line(str,
				obj->triangles + obj->num_triangles * 9);
			obj->num_triangles++;
		}
		else
			(*str)++;
	}
}

/*
** Parses obj data from obj string saving all objects into t_obj_content*
*/

void					l3d_obj_str_parse(char *str, t_obj *obj)
{
	ft_memset(obj, 0, sizeof(*obj));
	l3d_obj_parse_single_obj(&str, obj);
}

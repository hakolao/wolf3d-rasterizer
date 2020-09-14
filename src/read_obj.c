/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 17:03:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/14 18:16:02 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void					read_obj_vec3_line(char **str, t_vec3 res)
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

void					read_obj_vec2_line(char **str, t_vec3 res)
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

void					read_obj_triangle_line(char **str, uint32_t triangle[3][3])
{
	char	*tmp;
	tmp = *str;
	while (*tmp && (*tmp == '/' || ft_isdigit(*tmp) || *tmp == ' '))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in triangle data");
	triangle[0][0] = ft_atoi(*str);
	ft_scroll_over(str, '/');
	triangle[0][1] = ft_atoi(*str);
	ft_scroll_over(str, '/');
	triangle[0][2] = ft_atoi(*str);
	ft_scroll_over(str, ' ');
	triangle[1][0] = ft_atoi(*str);
	ft_scroll_over(str, '/');
	triangle[1][1] = ft_atoi(*str);
	ft_scroll_over(str, '/');
	triangle[1][2] = ft_atoi(*str);
	ft_scroll_over(str, ' ');
	triangle[2][0] = ft_atoi(*str);
	ft_scroll_over(str, '/');
	triangle[2][1] = ft_atoi(*str);
	ft_scroll_over(str, '/');
	triangle[2][2] = ft_atoi(*str);
	tmp = *str;
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid triangle, read_obj expect faces to be "
			"triangles e.g. (1/1/1/ 2/2/2 3/3/3\n");
	ft_scroll_over(str, '\n');
}

void					parse_obj_str(char *str, t_obj_result *obj)
{
	ft_memset(obj, 0, sizeof(*obj));
	while (*str)
	{
		if (*str == 'v' && *(str + 1) == ' ')
		{
			str += 2;
			read_obj_vec3_line(&str, obj->v[obj->num_vertices++]);
		}
		else if (*str == 'v' && *(str + 1) == 't' && *(str + 2) == ' ')
		{
			str += 3;
			read_obj_vec2_line(&str, obj->vt[obj->num_v_text_coords++]);
		}
		else if (*str == 'v' && *(str + 1) == 'n' && *(str + 2) == ' ')
		{
			str += 3;
			read_obj_vec3_line(&str, obj->vn[obj->num_v_normals++]);
		}
		else if (*str == 'f' && *(str + 1) == ' ')
		{
			str += 2;
			read_obj_triangle_line(&str, obj->triangles[obj->num_triangles++]);
		}
		else
			str++;
	}
}

t_3d_object				*read_object_file(const char *filename)
{
	t_file_contents	*obj_file;
	t_obj_result	obj_result;

	error_check(!(obj_file = read_file(filename)), "Failed to read file");
	parse_obj_str((char*)obj_file->buf, &obj_result);
	error_check(!is_valid_obj_result(&obj_result),
		"Invalid number of v, vt, vn or triangles");
	destroy_file_contents(obj_file);
	return (create_3d_object(&obj_result));
}
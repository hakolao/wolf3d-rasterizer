/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 17:03:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/28 17:02:53 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_bool			is_valid_obj(t_obj *obj)
{
	int				i;
	int				j;
	uint32_t		max_v_index;
	uint32_t		max_vt_index;
	uint32_t		max_vn_index;

	i = -1;
	max_v_index = 0;
	max_vt_index = 0;
	max_vn_index = 0;
	while (++i < (int)obj->num_triangles)
	{
		j = -1;
		while (++j < 3)
		{
			max_v_index = max_v_index > obj->triangles[i * 9 + j * 3 + 0] ?
				max_v_index : obj->triangles[i * 9 + j * 3 + 0];
			max_vt_index = max_vt_index > obj->triangles[i * 9 + j * 3 + 1] ?
				max_vt_index : obj->triangles[i * 9 + j * 3 + 1];
			max_vn_index = max_vn_index > obj->triangles[i * 9 + j * 3 + 2] ?
				max_vn_index : obj->triangles[i * 9 + j * 3 + 2];
		}
	}
	return (max_v_index == obj->num_vertices &&
			max_vt_index == obj->num_v_text_coords &&
			max_vn_index == obj->num_v_normals );
}

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

void					read_obj_triangle_line(char **str, uint32_t *triangle)
{
	char	*tmp;
	tmp = *str;
	while (*tmp && (*tmp == '/' || ft_isdigit(*tmp) || *tmp == ' '))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid characters in triangle data");
	*(triangle + 0) = ft_atoi(*str);
	ft_scroll_over(str, '/');
	*(triangle + 1) = ft_atoi(*str);
	ft_scroll_over(str, '/');
	*(triangle + 2) = ft_atoi(*str);
	ft_scroll_over(str, ' ');
	*(triangle + 3) = ft_atoi(*str);
	ft_scroll_over(str, '/');
	*(triangle + 4) = ft_atoi(*str);
	ft_scroll_over(str, '/');
	*(triangle + 5) = ft_atoi(*str);
	ft_scroll_over(str, ' ');
	*(triangle + 6) = ft_atoi(*str);
	ft_scroll_over(str, '/');
	*(triangle + 7) = ft_atoi(*str);
	ft_scroll_over(str, '/');
	*(triangle + 8) = ft_atoi(*str);
	tmp = *str;
	while (ft_isdigit(*tmp))
		tmp++;
	if (*tmp != '\n')
		error_check(true, "Invalid triangle, read_obj expect faces to be "
			"triangles e.g. (1/1/1/ 2/2/2 3/3/3\n");
	ft_scroll_over(str, '\n');
}

void					parse_obj_str(char *str, t_obj_content *obj)
{
	t_obj	*o;

	ft_memset(obj, 0, sizeof(*obj));
	while (*str)
	{
		while (*str != 'o' && *(str + 1) != ' ')
			str++;
		if (*str)
		{
			o = &obj->objects[obj->num_objects];
			error_check(!(o->v = malloc(sizeof(t_vec3) * MAX_OBJ_VERTICES)),
				"Failed to malloc obj vs");
			error_check(!(o->vt = malloc(sizeof(t_vec2) * MAX_OBJ_VERTICES)),
				"Failed to malloc obj v textures");
			error_check(!(o->vn = malloc(sizeof(t_vec3) * MAX_OBJ_VERTICES)),
				"Failed to malloc obj v normals");
			error_check(!(o->triangles =
				malloc(sizeof(uint32_t) * 9 * MAX_OBJ_TRIANGLES)),
				"Failed to malloc obj triangles");
			while (*str)
			{
				if (*str == 'v' && *(str + 1) == ' ')
				{
					str += 2;
					read_obj_vec3_line(&str, o->v[o->num_vertices++]);
				}
				else if (*str == 'v' && *(str + 1) == 't' && *(str + 2) == ' ')
				{
					str += 3;
					read_obj_vec2_line(&str, o->vt[o->num_v_text_coords++]);
				}
				else if (*str == 'v' && *(str + 1) == 'n' && *(str + 2) == ' ')
				{
					str += 3;
					read_obj_vec3_line(&str, o->vn[o->num_v_normals++]);
				}
				else if (*str == 'f' && *(str + 1) == ' ')
				{
					str += 2;
					read_obj_triangle_line(&str, o->triangles + o->num_triangles * 9);
					o->num_triangles++;
				}
				else
					str++;
			}
			obj->num_objects++;
		}
	}
}

void				read_objects_to_scene_data(t_scene_data *data,
					const char *filename)
{
	t_file_contents	*obj_file;
	t_obj_content	obj_content;
	int				i;

	error_check(!(obj_file = read_file(filename)), "Failed to read file");
	parse_obj_str((char*)obj_file->buf, &obj_content);
	i = -1;
	while (++i < (int)obj_content.num_objects)
		error_check(!is_valid_obj(&obj_content.objects[i]),
			"Invalid number of v, vt, vn or triangles");
	destroy_file_contents(obj_file);
	data->num_objects = (int)obj_content.num_objects;	
	obj_content_to_scene_data(data, &obj_content);
}

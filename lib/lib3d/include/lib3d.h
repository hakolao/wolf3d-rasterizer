/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:07:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/24 15:07:20 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB3D_H
# define LIB3D_H

# include "libgmatrix.h"

# define EPSILON 0.00000001

typedef struct		s_is_calc
{
	t_vec4		ab;
	t_vec4		ac;
	t_vec3		normal;
	t_vec3		ax;
	t_vec3		cross_dir_ax;
	float		det;
	float		invdet;
}					t_is_calc;

typedef struct		s_vertex
{
	t_vec4			position;
	t_vec4			normal;
	unsigned int	color;
}					t_vertex;	

typedef struct		s_triangle
{
	t_vertex	a;
	t_vertex	b;
	t_vertex	c;
	t_vec4		center; //consider if center calculated when mesh loaded
	//or center calculated when needed
	t_vec4		normal;
}					t_triangle;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			dir;
}					t_ray;


typedef struct		s_intersection
{
	float			u;
	float			v;
	float			t;
	float			det;
}					t_intersection;

#endif
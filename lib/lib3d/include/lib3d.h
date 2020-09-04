/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:07:11 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/01 18:07:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB3D_H
# define LIB3D_H

# include "libgmatrix.h"
# include <stdarg.h>

# define EPSILON 0.00000001

typedef struct s_temp_calc
{
	t_vec4		ab;
	t_vec4		ac;
	t_vec3		normal;
	t_vec3		ax;
	t_vec3		cross_dir_ax;
	float		det;
	float		invdet;
}					t_temp_calc;

typedef struct		s_vertex
{
	t_vec4			position;
	t_vec3			normal;
	unsigned int	color;
}					t_vertex;	

/*
**	The box structures are rectangular and consist of points that for the
**	boundary. The top and bot part refer to the top and bottom rectangles
**	(or lines in 2d case). The rectangles are read counter-clock-wise order
**	starting from smallest coordinate. The a,b,c,d indices correspond to a,b,c,d
**	on the other side.
*/

typedef struct		s_box3d
{
	t_vec3			topa;
	t_vec3			topb;
	t_vec3			topc;
	t_vec3			topd;
	t_vec3			bota;
	t_vec3			botb;
	t_vec3			botc;
	t_vec3			botd;
}					t_box3d;

typedef struct		s_box2d
{
	t_vec2			topa;
	t_vec2			topb;
	t_vec2			bota;
	t_vec2			botb;
}					t_box2d;

/*
**	Triangle structure includes references to its vertices in an array for
**	Easier iteration. The vertex pointers directly point at the corresponding
**	vertices in the mesh vertex list.	
*/

typedef struct		s_triangle
{
	t_vertex	*vtc[3];
	t_vec3		center; //consider if center calculated when mesh loaded
						//or center calculated when needed
	t_vec3		normal;
}					t_triangle;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			dir;
	t_vec3			normalized_dir;
}					t_ray;


typedef struct		s_intersection
{
	float			u;
	float			v;
	float			t;
	float			det;
}					t_intersection;

t_bool						triangle_intersection(t_triangle *triangle,
													t_ray *ray,
													t_intersection *is);
t_ray						new_ray(t_vec3 origin, t_vec3 direction);

#endif
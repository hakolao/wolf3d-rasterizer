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

#endif
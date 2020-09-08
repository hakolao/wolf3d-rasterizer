/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:37:03 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/08 23:50:49 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"
#include "mu_test.h"

/*
** ToDo
*/

const char	*test_matrix_ops()
{
	t_mat4	id;
	t_mat4	rot;
	t_mat4	persp;
	t_mat4	transl;
	char	res1[256];
	char	res2[256];
	char	res3[256];
	char	res4[256];

	ft_printf("Testing Matrix operations\n");
	ml_matrix4_id(id);
	mu_assert("Id matrix is wrong 1", id[0][0] == 1 &&
		id[1][1] == 1 && id[2][2] == 1 && id[3][3] == 1);
	ml_matrix4_to_str(id, res1);
	mu_assert("Id matrix is wrong 2",
		ft_strequ(res1,
		"Matrix4:\n"
		"| 1.000 | 0.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 1.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 1.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	ml_matrix4_rotate_z(id, ml_rad(90.0f), rot);
	ml_matrix4_to_str(rot, res2);
	mu_assert("rot matrix is wrong",
		ft_strequ(res2,
		"Matrix4:\n"
		"|-0.000 |-1.000 | 0.000 | 0.000 |\n"
		"| 1.000 |-0.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 1.000 | 0.000 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	ml_matrix4_projection(ML_ORTHOGRAPHIC, (t_canvas){.fov = 45.0f,
		.width = 1280, .height = 720, .near = 0.1f, .far = 10.0f}, persp);
	ml_matrix4_to_str(persp, res3);
	mu_assert("orth matrix is wrong",
		ft_strequ(res3,
		"Matrix4:\n"
		"| 0.002 | 0.000 | 0.000 |-1.000 |\n"
		"| 0.000 | 0.003 | 0.000 |-1.000 |\n"
		"| 0.000 | 0.000 |-0.202 |-1.020 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	ml_matrix4_projection(ML_PERSPECTIVE, (t_canvas){.fov = 45.0f,
		.width = 1280, .height = 720, .near = 0.1f, .far = 10.0f}, persp);
	ml_matrix4_to_str(persp, res3);
	mu_assert("persp matrix is wrong",
		ft_strequ(res3,
		"Matrix4:\n"
		"| 1.358 | 0.000 | 0.000 | 0.000 |\n"
		"| 0.000 | 2.414 | 0.000 | 0.000 |\n"
		"| 0.000 | 0.000 |-1.020 |-0.202 |\n"
		"| 0.000 | 0.000 |-1.000 | 0.000 |\n"));
	ml_matrix4_translation(5.0, 6.0, 2.0, transl);
	ml_matrix4_to_str(transl, res4);
	mu_assert("translation matrix is wrong",
		ft_strequ(res4,
		"Matrix4:\n"
		"| 1.000 | 0.000 | 0.000 | 5.000 |\n"
		"| 0.000 | 1.000 | 0.000 | 6.000 |\n"
		"| 0.000 | 0.000 | 1.000 | 2.000 |\n"
		"| 0.000 | 0.000 | 0.000 | 1.000 |\n"));
	return (0);
}

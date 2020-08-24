/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:36:50 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 20:15:37 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgmatrix.h"
#include "mu_test.h"
#include "libft.h"

const char *test_vector_ops() {
	t_vec3	res;
	t_vec3	v;
	t_vec3	v2;
	float	mag;
	float	dot;

	ft_printf("Testing Vector operations\n");
	v[0] = 1;
	v[1] = 2;
	v[2] = 35;
	ml_vector3_mul(v, 2, res);
	mu_assert("error, Vector != (2, 4, 70, 2)",
		(res[0] == 2 && res[1] == 4 && res[2] == 70));
	ml_vector3_mul(res, -1, res);
	mu_assert("error, Vector != (-2, -4, -70, 0)",
		(res[0] == -2 && res[1] == -4 && res[2] == -70));
	v2[0] = 3;
	v2[1] = 4;
	v2[2] = 5;
	mag = ml_vector3_mag(v2);
	mu_assert("error, mag != what it should",
			 round(mag * 1000.0) / 1000.0 == 7.071000);
	v[0] = 2;
	v[1] = 1;
	v[2] = 3;
	dot = ml_vector3_dot(v2, v);
	mu_assert("error, v2 dotProduct != what it should",
		round(dot * 1000.0) / 1000.0 == 6 + 4 + 15 + 0);
	v2[0] = 5;
	v2[1] = 5;
	v2[2] = 5;
	ml_vector3_cross(v, v2, res);
	mu_assert("error, v3 crossProduct != what it should",
		(res[0] == -10 && res[1] == 5 && res[2] == 5));
	return (0);
}

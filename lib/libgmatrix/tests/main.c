/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 19:23:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/01 18:11:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mu_test.h"
#include "tests.h"

int tests_run = 0;
t_test_state test_states[MAX_TESTS];

static void all_tests() {
	mu_run_test(test_vector_ops);
	mu_run_test(test_matrix_ops);
}

int main(void) {
	t_bool				success;
	int					failures;
	int					i;

	all_tests();
	success = true;
	failures = 0;
	i = -1;
	while (++i < tests_run)
	{
		if (!test_states[i].success)
		{
			success = false;
			failures++;
		}
	}
	ft_printf("===========\n");
	if (!success) {
		ft_printf("%d out of %d TESTS FAILED\n", failures, tests_run);
	} else {
		ft_printf("ALL %d TESTS PASSED\n", tests_run);
	}
	return !success;
}

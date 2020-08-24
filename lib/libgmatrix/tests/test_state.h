/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 20:05:49 by ohakola           #+#    #+#             */
/*   Updated: 2020/08/16 20:06:25 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_STATE_H
# define TEST_STATE_H

	typedef struct s_test_state
	{
		bool		success;
		const char	*result;
		int			id;
	}				t_test_state;

t_test_state		test_state_create(bool success, const char *result, int id);

#endif

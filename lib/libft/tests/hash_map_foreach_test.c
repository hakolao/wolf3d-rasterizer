/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map_foreach_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 20:34:57 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/30 01:16:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oh_test.h"
#include "libft.h"

void			foo(void *val)
{
	val = NULL;
}

const char		*test_hash_map_foreach(void)
{
	t_hash_table	*hash_map;
	int				size;
	int				key;
	int				hashed_key;

	size = 128;
	hash_map = hash_map_create(size);
	key = (int)"okko";
	hashed_key = hash_map_hash(hash_map, key);
	hash_map_add(hash_map, key, (void*)"an aristocrat");
	OH_ASSERT("Hash pre foreach did not work",
		hash_map_get(hash_map, key) != NULL);
	hash_map_foreach(hash_map, foo);
	OH_ASSERT("Hash foreach did not work",
		hash_map_get(hash_map, key) == NULL);
	hash_map_destroy(hash_map);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/16 16:50:19 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/18 21:45:24 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_pool.h"
#include "libft.h"

t_thread_work			*thread_pool_work_create(t_thread_work_func work_func,
							void *params)
{
	t_thread_work	*work;

	error_check(work_func == NULL, "Work func for work was NULL");
	error_check(!(work = malloc(sizeof(*work))), "Failed to malloc work");
	work->work_func = work_func;
	work->params = params;
	work->next = NULL;
	return (work);
}

void					thread_pool_work_destroy(t_thread_work *work)
{
	if (work == NULL)
		return ;
	free(work);
	work = NULL;
}

t_thread_work			*thread_pool_work_get(t_thread_pool *thread_pool)
{
	t_thread_work	*work;

	if (thread_pool == NULL)
		return (NULL);
	work = thread_pool->work_first;
	if (work == NULL)
		return (NULL);
	if (work->next == NULL)
	{
		thread_pool->work_first = NULL;
		thread_pool->work_last = NULL;
	}
	else
		thread_pool->work_first = work->next;
	return (work);
}

t_bool					thread_pool_worker_synchronize(t_thread_pool *tp)
{
	t_thread_work	*work;
	t_bool			work_was_done;

	work_was_done = false;
	pthread_mutex_lock(&tp->work_mutex);
	while (!tp->stop && tp->num_jobs == 0)
		pthread_cond_wait(&tp->work_to_process, &tp->work_mutex);
	if (tp->stop)
		return (false);
	work = thread_pool_work_get(tp);
	tp->num_threads_working++;
	pthread_mutex_unlock(&tp->work_mutex);
	if (work != NULL)
	{
		work->work_func(work->params);
		thread_pool_work_destroy(work);
		work_was_done = true;
	}
	pthread_mutex_lock(&tp->work_mutex);
	tp->num_jobs -= work_was_done ? 1 : 0;
	tp->num_threads_working--;
	if (!tp->stop && tp->num_threads_working == 0 && tp->num_jobs == 0)
		pthread_cond_signal(&tp->threads_vacant_all);
	pthread_mutex_unlock(&tp->work_mutex);
	return (true);
}

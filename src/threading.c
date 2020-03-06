/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threading.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:38:22 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/06 19:57:03 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "threading.h"

void		spin_down_threads(int count, pthread_t *threads)
{
	for (int i = 0; i < count; i++)
	{
		pthread_kill(threads[i], SIGTERM);
		free(threads[i]);
	}
}

pthread_t	*spin_up_thread(pthread_attr_t *attr, void *(*main)(void *), void *arg)
{
	pthread_t *thread;

	thread = malloc(sizeof(pthread_t));
	if (thread)
	{
		if (pthread_create(thread, attr, main, arg) != 0)
		{
			handle_error("spin_up_thread", "couldn't spin up thread",
					strerror(errno), ERR_CRIT);
			free(thread);
			return (NULL);
		}
	}
	else
		handle_error("spin_up_thread", strerror(errno), NULL, ERR_CRIT);
	return (thread);
}

pthread_t	*spin_up_threads(int count, pthread_attr_t *attr,
		void *(*main)(void *), void *arg)
{
	pthread_t		*thread;
	pthread_t		*thread_tab;

	thread_tab = malloc(sizeof(pthread_t) * count);
	if (thread_tab)
	{
		for (int i = 0; i < count; i++)
		{
			thread = spin_up_thread(attr, main, arg);
			if (!thread)
			{
				handle_error("spin_up_threads", "couldn't spin up threads", strerror(errno), ERR_CRIT);
				spin_down_threads(i, thread_tab);
				return(NULL);
			}
		}
	}
	handle_error("spin_up_threads", strerror(errno), NULL, ERR_CRIT);
	return (thread_tab);
}

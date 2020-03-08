/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threading.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:38:22 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 22:20:08 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "common/threading.h"

void		spin_down_threads(int amount, pthread_t *threads)
{
	for (int i = 0; i < amount; i++)
	{
		pthread_kill(threads[i], SIGTERM);
	}
	free(threads);
}

pthread_t	spin_up_thread(pthread_attr_t *attr, void *(*main)(void *), void *arg)
{
	pthread_t	thread;

	if (pthread_create(&thread, attr, main, arg) != 0)
	{
		handle_error("spin_up_thread", "couldn't spin up thread",
				strerror(errno), ERR_CRIT);
		return (NULL);
	}
	return (thread);
}

pthread_t	*spin_up_threads(int amount, int maxamount, pthread_attr_t *attr,
		void *(*main)(void *), void *arg)
{
	pthread_t		thread;
	pthread_t		*thread_tab;

	thread_tab = malloc(sizeof(pthread_t) * maxamount);
	if (thread_tab)
	{
		for (int i = 0; i < amount; i++)
		{
			thread = spin_up_thread(attr, main, arg);
			if (!thread)
			{
				handle_error("spin_up_threads", "couldn't spin up threads", strerror(errno), ERR_CRIT);
				spin_down_threads(i, thread_tab);
				return(NULL);
			}
			else
				thread_tab[i] = thread;
		}
	}
	else
		handle_error("spin_up_threads", strerror(errno), NULL, ERR_CRIT);
	return (thread_tab);
}

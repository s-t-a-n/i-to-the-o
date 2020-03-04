/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 20:21:25 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/04 00:24:12 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

static int		process_request(t_package_enroute *package, t_client *client)
{
	package = NULL;
	client = NULL;
	return (0);
}

static void		*worker_requests(void *arg)
{
	t_client			*client = (t_client *)arg;
	t_package_enroute	*package;

	while(client->state == NT_STATE_READY)
	{
		package = (t_package_enroute *)queue_safe_get(client->queue);
		process_request(package, client);
	}
	return (NULL);
}

static int		spin_up_threads(pthread_t *thread_tab, t_client *client)
{
	int ctr = 0;

	LOG_DEBUG("%s : %s\n", "client", "spinning up threads!");

	while (ctr < NT_WORKERS_DEF)
	{
		if (pthread_create(&thread_tab[ctr], NULL, worker_requests,
					(void *)client) != 0)
		{
			int undo_ctr = 0;
			while (undo_ctr < ctr)
			{
				pthread_kill(thread_tab[undo_ctr], SIGTERM);
				undo_ctr++;
			}
			return (0);
		}
		ctr++;
	}
	return (1);
}

void			shutdown_client(t_client *client)
{
	t_package_enroute	*package_en;

	LOG_DEBUG("%s\n", "shutting down client!");
	while(client->queue->size > 0)
	{
		package_en = queue_peek(client->queue);
		free(package_en->package->mem);
		free(package_en);
		queue_pop(client->queue);
	}
	queue_drop(client->queue);
	free(client->thread_tab);
	free(client);
}

t_client		*initialise_client(void)
{
	t_client	*client;

	client = malloc(sizeof(t_client));
	if (client)
	{
		client->queue = queue_create(NT_QUEUE_CAP);
		if (!client->queue)
		{
			handle_error("initialise_client", strerror(errno), NULL, ERR_CRIT);
			free(client);
			return(NULL);
		}
		client->thread_tab = malloc(sizeof(pthread_t) * NT_WORKERS_MAX);
		if (client->thread_tab)
		{
			if (!spin_up_threads(client->thread_tab, client))
			{
				handle_error("initialise_client", "couldn't spin up threads!",
						strerror(errno), ERR_CRIT);
				queue_drop(client->queue);
				free(client->thread_tab);
				free(client);
				return(NULL);
			}
		}
		else
		{
			handle_error("initialise_client", strerror(errno), NULL, ERR_CRIT);
			queue_drop(client->queue);
			free(client);
			return (NULL);
		}

	}
	else
		handle_error("initialise_client", strerror(errno), NULL, ERR_CRIT);
	return (client);
}

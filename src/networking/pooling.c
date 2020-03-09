/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/03 22:00:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 17:31:41 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/networking.h"
#include "networking/pooling.h"
#include "networking/conscript.h"

void	pool_destroy(t_pool *pool)
{
	free(pool->relays);
	free(pool->members);
	free(pool);
}

t_conscript		*is_client_known(t_pool *pool, t_conscript *conscript)
{
	// probably want to do authentication here
	// for now, ip checking is considered 'authentication'

	return (NULL);
	for (int i = 0; i < pool->membercount; i++)
	{
		if (pool->members[i])
		{
			// lock must be initialized
			if (pthread_mutex_trylock(&pool->members[i]->lock) == 0)
			{
				struct sockaddr_in	*ipv4_addr = (struct sockaddr_in *)&conscript->sockaddr_in;
				char ipv4_addr_str[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, (struct in_addr *)&ipv4_addr->sin_addr, ipv4_addr_str, INET_ADDRSTRLEN);

				struct sockaddr_in6	*ipv6_addr = (struct sockaddr_in6 *)&conscript->sockaddr_in;
				char ipv6_addr_str[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, (struct in_addr *)&ipv6_addr->sin6_addr, ipv6_addr_str, INET6_ADDRSTRLEN);
				if (strncmp(pool->members[i]->ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN) == 0)
					return(pool->members[i]);
				else if (strncmp(pool->members[i]->ipv6_addr, ipv6_addr_str, INET6_ADDRSTRLEN) == 0)
					return(pool->members[i]);
			}
		}
	}
	return(NULL);
}

int		pool_add_member(t_pool *pool, t_conscript *conscript)
{
	LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_member", "adding member to pool!");
	if (pool->membercount < POOL_MEMBR_COUNT)
	{
		pool->members[pool->membercount] = conscript;
		return (1);
	}
	else
	{
		LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_member", "Pool is full!");
		conscript_discharge(conscript);
		return (-1);
	}
	return (-1);
}

int		pool_add_relay(t_pool *pool, t_conscript *conscript)
{
	LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_relay", "adding relay to pool!");
	if (pool->relaycount < POOL_RELAY_COUNT)
	{
		int index = (pool->relaycount == 0) ? 0 : pool->relaycount - 1;
		pool->relays[index] = conscript;
		return (1);
	}
	else
	{
		LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_relay", "Pool is full!");
		conscript_discharge(conscript);
		return (-1);
	}
	return (-1);
}

t_pool	*pool_initialise(int relaycount, int membercount)
{
	t_pool	*pool;

	pool = malloc(sizeof(t_pool));
	if (pool)
	{
		pool->relays = malloc(sizeof(t_conscript) * relaycount);
		if (!pool->relays)
		{
			free(pool);
			handle_error("pool_initiate", strerror(errno), NULL, ERR_CRIT);
			return (NULL);
		}
		else
		{
			for (int i = 0; i < relaycount; i++)
			{
				pool->relays[i] = NULL;
			}
		}
		pool->members = malloc(sizeof(t_conscript) * membercount);
		if (!pool->members)
		{
			free(pool);
			free(pool->relays);
			handle_error("pool initiate", strerror(errno), NULL, ERR_CRIT);
			return (NULL);
		}
		else
		{
			for (int i = 0; i < membercount; i++)
				pool->members[i] = NULL;
		}
	}
	return(pool);
}

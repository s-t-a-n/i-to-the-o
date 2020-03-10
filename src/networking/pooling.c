/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/03 22:00:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 16:19:45 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "networking/constants.h"
#include "networking/pooling.h"
#include "networking/conscript.h"
#include "networking/network.h"

void	pool_destroy(t_pool *pool)
{
	free(pool->relays);
	free(pool->members);
	free(pool);
}

t_conscript		*is_client_a_member(t_pool *pool, t_conscript *conscript, char *ipv4_addr_str, char *ipv6_addr_str)
{
	// probably want to do authentication here
	// for now, ip checking is considered 'authentication'

	for (int i = 0; i < pool->membercount; i++)
	{
		if (pool->members[i])
		{
			// lock must be initialized
			if (pthread_mutex_trylock(&pool->members[i]->lock) == 0)
			{
				LOG_DEBUG("Thread %d : %s : %s : %s : %s : %s\n", (int)pthread_self(), "is_client_known", "incoming client has ip4", ipv4_addr_str, ", ipv6: ", ipv6_addr_str);

				if (strncmp(pool->members[i]->ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN) == 0)
					return(pool->members[i]);
				else if (strncmp(pool->members[i]->ipv6_addr, ipv6_addr_str, INET6_ADDRSTRLEN) == 0)
					return(pool->members[i]);
			}
			else
			{
				printf("no fun with lock  : %i\n", pthread_mutex_trylock(&pool->members[i]->lock));
			}
		}
	}
	return(NULL);
	conscript = NULL; //boilerplate
}

t_conscript		*is_client_a_relay(t_pool *pool, t_conscript *conscript, char *ipv4_addr_str, char *ipv6_addr_str)
{
	for (int i = 0; i < pool->relaycount; i++)
	{
		if (pool->relays[i])
		{
			// lock must be initialized
			if (pthread_mutex_trylock(&pool->relays[i]->lock) == 0)
			{
				LOG_DEBUG("Thread %d : %s : %s : %s : %s : %s\n", (int)pthread_self(), "is_client_known", "incoming client has ip4", ipv4_addr_str, ", ipv6: ", ipv6_addr_str);

				if (strncmp(pool->relays[i]->ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN) == 0)
					return(pool->relays[i]);
				else if (strncmp(pool->relays[i]->ipv6_addr, ipv6_addr_str, INET6_ADDRSTRLEN) == 0)
					return(pool->relays[i]);
			}
			else
			{
				printf("no fun with lock  : %i\n", pthread_mutex_trylock(&pool->relays[i]->lock));
			}
		}
	}
	return(NULL);
	conscript = NULL; //boilerplate
}

t_conscript		*is_client_in_pool(t_pool *pool, t_conscript *conscript)
{
	t_conscript *known_conscript;
	char *ipv4_addr_str = get_ipv4_str(&conscript->sockaddr_in);
	char *ipv6_addr_str = get_ipv6_str(&conscript->sockaddr_in);

	if ((known_conscript = is_client_a_member(pool, conscript, ipv4_addr_str, ipv6_addr_str)))
		return (known_conscript);
	else if ((known_conscript = is_client_a_relay(pool, conscript, ipv4_addr_str, ipv6_addr_str)))
		return (known_conscript);
	else
		return (NULL);
}

int		pool_add_member(t_pool *pool, t_conscript *conscript)
{
	LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_member", "adding member to pool!");
	if (pool->membercount < POOL_MEMBR_COUNT)
	{
		conscript->ipv4_addr = get_ipv4_str(&conscript->sockaddr_in);
		conscript->ipv6_addr = get_ipv6_str(&conscript->sockaddr_in);
		conscript->container_in = container_create(CONTAINER_MEMCAP_DEF, &conscript->addrinfo, conscript->socketfd, 0);
		pool->members[pool->membercount] = conscript;
		pthread_mutex_init(&conscript->lock, 0);
		pool->membercount++;
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
		pool->relaycount++;
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
		pool->membercount = 0;
		pool->relaycount = 0;
	}
	return(pool);
}

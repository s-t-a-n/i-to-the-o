/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   conscript.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:38:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 17:12:01 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/conscript.h"

void			conscript_discharge(t_conscript *conscript)
{
	pthread_mutex_destroy(&conscript->lock);
	free(conscript);
}

t_conscript		*conscript_initiate(struct sockaddr_in sockaddr_in, socklen_t socklen)
{
	t_conscript *conscript;

	conscript = malloc(sizeof(t_conscript));
	if (conscript)
	{
		pthread_mutex_init(&conscript->lock, 0);
		//conscript->addrinfo = (struct addrinfo)0;
		conscript->sockaddr_in = sockaddr_in;
		conscript->socklen = socklen;
		conscript->ipv4_addr = NULL;
		conscript->ipv6_addr = NULL;
		conscript->socketfd = -1;
	}
	return(conscript);
}

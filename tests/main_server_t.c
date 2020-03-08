/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_server_t.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 21:19:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 00:24:59 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/networking.h"
#include "ito_internal.h"

int	main(void)
{
	t_server	*server;
	t_network	network;

	server = initialise_server(&network);
	if (server)
	{
		sleep(60);
		LOG_DEBUG("%s\n", "stopping server!");
		server->state = NT_STATE_STOP;
		sleep(10);
		shutdown_server(server);
	}
}

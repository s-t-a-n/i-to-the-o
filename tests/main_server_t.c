/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_server_t.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 21:19:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/03 00:29:43 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"
#include "ito_internal.h"

int	main(void)
{
	t_server	*server;

	server = initialise_server();
	if (server)
	{
		sleep(10);
		LOG_DEBUG("%s\n", "stopping server!");
		server->state = NT_STATE_STOP;
		shutdown_server(server);
	}
}

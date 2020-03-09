/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_server_t.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 21:19:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 17:28:25 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/networking.h"
#include "ito_internal.h"

int	main(void)
{
	t_network	*network;

	network = network_initialise();
	sleep(60);

	LOG_DEBUG("%s\n", "stopping server!");
	network->server->state = NT_STATE_STOP;

	sleep(10);
	server_shutdown(network->server);
}

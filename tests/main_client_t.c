/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_client_t.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 19:18:59 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:36:50 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "limits.h"
#include "networking/constants.h"
#include "networking/network.h"
#include "networking/framing.h"
#include "networking/container.h"
#include "networking/client.h"
#include "networking/requests.h"
#include "ito_internal.h"

typedef struct			s_data
{
	double				x;
	double				y;
	char				*name;
	int					id;
	int					flags;
	unsigned long long	fatty;
	long long			sfatty;
}						t_data;

void					join_pool(t_client *client, struct addrinfo *info, int descriptor)
{
	t_container		*container;

	container = container_create(512, info, descriptor, 0);

	frame_insert(container, JOIN);
	dump_container(container);
	queue_safe_add(client->queue, (void *)container);
}

void					bombard(t_client *client, struct addrinfo *info, int descriptor)
{
	t_data				s_send;
	t_package			*package;
	t_container			*container;

	container = malloc(sizeof(t_container));

	s_send.x = 5.5;
	s_send.y = 9.4;
	s_send.name = "wara waka";
	s_send.id = 42;
	s_send.flags = 1<<4 | 1<<2 | 1<<18;
	s_send.fatty = ULLONG_MAX;
	s_send.sfatty = LLONG_MAX;

	const char *formatstr = "%f%f%s%i%i%llu%lli";
	package = ito_compile_package(formatstr, s_send.x, s_send.y, s_send.name, s_send.id, s_send.flags, s_send.fatty, s_send.sfatty);
	//dump_package(package);

	container = container_create(512, info, descriptor, 0);
	container_insert_package(package, container);
	frame_insert(container, PACKAGE);
	//dump_container(container);
	//printf("client: %c : %c : %c : %c\n",container->vector->mem[0],container->vector->mem[1],container->vector->mem[2], container->vector->mem[3]);
	queue_safe_add(client->queue, (void *)container);
	free(package);
}

int						main(void)
{
	t_client			*client;
	//int					count = 10000;

	client = client_initialise();
	if (client)
	{
		client_send_request(client, JOIN, strdup("127.0.0.1"));
		client_send_request(client, JOIN, strdup("127.0.0.1"));
		client_send_request(client, JOIN, strdup("127.0.0.1"));
		client_send_request(client, JOIN, strdup("127.0.0.1"));
		client_send_request(client, JOIN, strdup("127.0.0.1"));
		//struct addrinfo *info = conv_to_addrinfo(strdup("127.0.0.1"), NT_PORT);
		//int descriptor = open_connection_sync(info);
		//join_pool(client, info, descriptor);
		//if (info && descriptor >= 0)
		//{
		//	for (int i = 0; i < count; i++)
		//	{
		//		bombard(client, info, descriptor);
		//		usleep(400);
		//	}
		//}
		//else
		//{
		//	handle_error("main_client_t Main", "couldn't connect to client!", NULL, ERR_CRIT);
		//	return(-1);
		//}
		sleep(1);
		LOG_DEBUG("%s\n", "stopping client!");
		client->state = NT_STATE_STOP;
		sleep(1);
		client_shutdown(client);
	}
	else
		handle_error("main_client_t Main", "couldn't initialise client!", NULL, ERR_CRIT);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_client_t.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 19:18:59 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/04 23:45:39 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "limits.h"
#include "networking.h"
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

void					bombard(t_client *client)
{
	t_data				s_send;
	t_package			package;
	t_package_nt		*package_nt;

	package_nt = malloc(sizeof(t_package_nt));

	s_send.x = 5.5;
	s_send.y = 9.4;
	s_send.name = "wara waka";
	s_send.id = 42;
	s_send.flags = 1<<4 | 1<<2 | 1<<18;
	s_send.fatty = ULLONG_MAX;
	s_send.sfatty = LLONG_MAX;

	const char *formatstr = "%f%f%s%i%i%llu%lli";
	ito_compile_package(&package, formatstr, s_send.x, s_send.y, s_send.name, s_send.id, s_send.flags, s_send.fatty, s_send.sfatty);
	//dump_package(&package);

	package_nt->mem = package.mem;
	package_nt->index = package.index;
	package_nt->addrinfo = conv_to_addrinfo(strdup("127.0.0.1"), NT_PORT);

	// open connection here
	if (package_nt->addrinfo)
		queue_safe_add(client->queue, (void *)package_nt);
}

int						main(void)
{
	t_client			*client;
	int					count = 10000;


	client = initialise_client();
	if (client)
	{
		for (int i = 0; i < count; i++)
		{
			bombard(client);
			usleep(400);
		}
		LOG_DEBUG("%s\n", "stopping client!");
		client->state = NT_STATE_STOP;
		sleep(1);
		shutdown_client(client);
	}
}

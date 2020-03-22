/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   api.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 17:36:06 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:32:56 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"
#include "packaging/packaging.h"
#include "networking/constants.h"
#include "networking/network.h"
#include "networking/requests.h"

t_package	*ito_compile_package(const char * restrict formatstr, ...)
{
	va_list		args;
	t_package	*package;

	LOG_DEBUG("%s : %s\n","ito_compile_package", "compiling package!");

	if (!(package = package_initialise()))
		return (NULL);
	va_start(args, formatstr);
	while(*formatstr)
	{
		if (*formatstr == '%')
		{
			formatstr++;
			if(!add_data_to_package(&args, formatstr, package))
			{
				handle_error("ito_compile_package", strerror(errno), NULL, ERR_CRIT);
				package_destroy(package);
				return (NULL);
			}
		}
		formatstr++;
	}
	va_end(args);
	return(package);
}

int		ito_send_package(t_ito *ito, t_package *package)
{
	ito = NULL;
	package = NULL;
	return(0);
}

// if 255.255.255.255 broadcast to local network and find clients
t_ito	*ito_quick_init(char *ipv4)
{
	t_ito	*ito;

	ito = malloc(sizeof(t_ito));
	if (ito)
	{
		ito->network = network_initialise();
		if (!ito->network)
		{
			handle_error("ito_quick_init", "ito failed :(", NULL, ERR_CRIT);
			free(ito);
			return(NULL);
		}
		else
		{
			if (!client_send_request(ito->network->client, JOIN, (void *)ipv4))
			{
				handle_error("ito_quick_init", "ito failed :(", NULL, ERR_CRIT);
				return (NULL);
			}
			else
				return (ito);
		}
	}
	else
		handle_error("ito_quick_init", strerror(errno), NULL, ERR_CRIT);
	return (ito);
	ipv4 = NULL;
}

t_ito	*ito_full_init(t_ito_specs *ito_specs)
{
	ito_specs = NULL;
	return (NULL);
}

int		ito_shutdown(t_ito *ito)
{
	ito = NULL;
	return (0);
}

int		ito_hook(t_ito *ito, t_ito_event event, int (*f_ptr)(), void *param)
{
	event = EV_JOIN;
	ito = NULL;
	f_ptr = NULL;
	param = NULL;
	return (0);
}

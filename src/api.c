/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   api.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 17:36:06 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/26 23:19:05 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"

int		ito_validate(t_package *package, const char * restrict formatstr)
{
	formatstr = NULL;
	package = NULL;
	return (0);
}

void	*ito_decompile_package(t_package *package, const char * restrict formatstr)
{
	formatstr = NULL;
	package = NULL;
	return (NULL);
}

int		ito_compile_package(t_package *package, const char * restrict formatstr, ...)
{
	va_list		args;

	va_start(args, formatstr);
	package->mem_cap = MEMCAP_DEF;
	package->index = 0;
	package->elem_count = 0;
	if (!(package->mem = (unsigned char *)malloc(MEMCAP_DEF + 1)))
		return (ITO_ERROR);
	while(*formatstr)
	{
		if (*formatstr == '%')
		{
			formatstr++;
			if(!add_data_to_package(&args, formatstr, package))
			{
				handle_error("ito_compile_package", strerror(errno), NULL, ERR_CRIT);
				return (ITO_ERROR);
			}
		}
		formatstr++;
	}
	va_end(args);
	return(ITO_SUCCESS);
}

int		ito_send_package(t_ito *ito, t_package *package)
{
	ito = NULL;
	package = NULL;
	return(0);
}

t_ito	*ito_quick_init(char *ip)
{
	ip = NULL;
	return (NULL);
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

int		ito_hook(t_ito *ito, int (*f_ptr)(), void *param)
{
	ito = NULL;
	f_ptr = NULL;
	param = NULL;
	return (0);
}

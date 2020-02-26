/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_and_logging.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/26 19:33:58 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/26 23:09:49 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"

void	handle_error(char *function, char *err_description, char *extra, int errcode)
{
	if (errcode == ERR_CRIT)
	{
			if (extra)
				dprintf(STDERR, "\e[31mCritical error!\e[39m -> %s : %s : %s\n",
						function, err_description, extra);
			else
				dprintf(STDERR, "\e[31mCritical error!\e[39m -> %s : %s\n",
						function, err_description);
#ifdef EXIT_ON_CRIT_ERROR
			exit(ITO_ERROR);
#endif
	}
	else if (errcode == ERR_WARN)
	{
			if (extra)
				dprintf(STDERR, "\e[31mWarning!\e[39m -> %s : %s : %s\n",
						function, err_description, extra);
			else
				dprintf(STDERR, "\e[31mWarning!\e[39m -> %s : %s\n", function,
						err_description);
	}
	else
	{
			dprintf(STDERR, "UNKOWN ERROR CODE: %i AFTER CALL FROM %s\n",
						errcode, function);
			abort();
	}
}

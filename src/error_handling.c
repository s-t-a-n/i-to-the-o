/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 18:03:09 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/25 19:20:17 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"

void	handle_error(char *function, char *err_description, char *extra, int errcode)
{
	extra = NULL;
	switch (errcode)
	{
		case ERR_CRIT:
			dprintf(STDERR, "\e[31mCritical error!\e[39m -> %s : %s\n", function, err_description);
#ifdef ABORT_ON_CRIT_ERROR
			abort();
#endif
			break;
		case ERR_WARN:
			dprintf(STDERR, "\e[31mWarning!\e[39m -> %s : %s\n", function, err_description);
			break;
		default:
			dprintf(STDERR, "UNKOWN ERROR CODE: %i AFTER CALL FROM %s\n", errcode, function);
			abort();
	}

}

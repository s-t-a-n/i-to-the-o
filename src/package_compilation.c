/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   package_compilation.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 18:58:07 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/25 19:19:48 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"

int		add_data_to_package(va_list *args, char op, t_package *package)
{
	args = NULL;
	package = NULL;
	switch(op)
	{
		case 'i':
			break;
		default:
			handle_error("handle_conversion", "unknown conversion specifier", NULL, ERR_CRIT);
			return (-1);
			break;
	}
	return (0);
}

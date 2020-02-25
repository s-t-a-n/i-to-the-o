/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito_internal.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 19:12:27 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/25 19:18:59 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITO_INTERNAL_H
# define ITO_INTERNAL_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*
** runtime defines (uncomment to disable behaviour)
*/
# define ABORT_ON_CRIT_ERROR

/*
** global defines
*/
# define ERR_CRIT		1
# define ERR_WARN		2

# define LOG_VERBOSE	1
# define LOG_DEBUG		2

# define ITO_ERROR		-1
# define ITO_SUCCESS	1

# define STDIN			0
# define STDOUT			1
# define STDERR			2

typedef struct      s_connection
{
	int				stub;
}                   t_connection;

typedef struct		s_ito
{
	int				stub;
}					t_ito;

typedef struct		s_ito_specs
{
	char			*ip_address;
	int				pool;
}					t_ito_specs;

typedef struct      s_package
{
    unsigned char   *mem;
    size_t          count;
    size_t          index;
}                   t_package;

/*
** package_compilation.c
*/

int     add_data_to_package(va_list *args, char op, t_package *package);

/*
** error_handling.c
*/
void    handle_error(char *function, char *err_description, char *extra, int errcode);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito_internal.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 19:12:27 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/01 20:09:31 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITO_INTERNAL_H
# define ITO_INTERNAL_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "ito.h"

/*
** runtime defines
*/
# define EXIT_ON_CRIT_ERROR 1
# define MEMCAP_DEF 100
# define MEMCAP_MAX 1000000 // no bigger than a MEMCAP_MAX (def: 1M) packages are allowed

/*
** debugging definitions/macros
** ripped from: https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
*/

# ifndef DEBUG
#  define DEBUG 0
# else
# define DEBUG 1
# endif

#define LOG_DEBUG(fmt, ...) \
            do { if (DEBUG) { fprintf(stdout, "\e[32mInfo:debug\e[39m -> "); fprintf(stdout, fmt, __VA_ARGS__); } } while (0)

/*
** global defines
*/

# define ERR_CRIT		0
# define ERR_WARN		1

# define ITO_ERROR		-1
# define ITO_SUCCESS	1

# define STDIN			0
# define STDOUT			1
# define STDERR			2

/*
** package_compilation.c
*/

int		add_data_to_package(va_list *args, const char * restrict formatstr, t_package *package);

/*
** package_decompilation.c
*/
int		get_data_from_package(va_list *args, const char * restrict formatstr, t_package *package);
/*
** error_and_logging.c
*/
void	handle_error(char *function, char *err_description, char *extra, int errcode);
#endif

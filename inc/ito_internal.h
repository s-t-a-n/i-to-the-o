/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito_internal.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 19:12:27 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/27 00:21:59 by sverschu      ########   odam.nl         */
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
** runtime defines (uncomment to disable behaviour)
*/
# define EXIT_ON_CRIT_ERROR

/*
** debugging macros
** ripped from: https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
*/

# ifndef DEBUG
#  define DEBUG 0
# else
# define DEBUG 1
# endif

#define log_debug(fmt, ...) \
            do { if (DEBUG) { fprintf(stdout, "\e[32mInfo:debug\e[39m -> "); fprintf(stdout, fmt, __VA_ARGS__); } } while (0)

/*
** global defines
*/
# define ERR_CRIT		0
# define ERR_WARN		1

# define LOG_INFO		0
# define LOG_VERBOSE	1
# define LOG_DEBUG		2

# define ITO_ERROR		-1
# define ITO_SUCCESS	1

# define STDIN			0
# define STDOUT			1
# define STDERR			2

typedef struct			s_uchar_buffer
{
	unsigned char		*mem;
	int					len;
}						t_uchar_buffer;

/*
** package_compilation.c
*/

int     add_data_to_package(va_list *args, const char * restrict formatstr, t_package *package);

/*
** error_and_logging.c
*/
void    handle_error(char *function, char *err_description, char *extra, int errcode);
#endif

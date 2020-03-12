/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito_internal.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 19:12:27 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/11 20:47:46 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITO_INTERNAL_H
# define ITO_INTERNAL_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "ito.h"

/*
** runtime defines
*/
# define EXIT_ON_CRIT_ERROR 1

/*
** debugging definitions/macros
** ripped from: https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
*/

# ifndef SILENT
#  define VERBOSE 1
# else
#  define VERBOSE 0
# endif

# ifndef DEBUG
#  define DEBUG 0
# else
#  define DEBUG 1
# endif

#define LOG_DEBUG(fmt, ...) \
            do { if (DEBUG) { fprintf(stdout, "\e[32mDebug:\e[39m T{%lu}-> ", pthread_self()); fprintf(stdout, fmt, __VA_ARGS__); } } while (0)

#define LOG_VERBOSE(fmt, ...) \
            do { if (VERBOSE) { fprintf(stdout, "\e[32mInfo:\e[39m -> "); fprintf(stdout, fmt, __VA_ARGS__); } } while (0)

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
** error_and_logging.c
*/
void					handle_error(char *function, char *err_description, char *extra, int errcode);

#endif

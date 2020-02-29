/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/23 19:00:56 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/27 00:14:11 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITO_H
# define ITO_H

#include <stddef.h>

/*
** Include this header to use ITTO.
*/

typedef struct      s_connection
{
    int             stub;
}                   t_connection;

typedef struct      s_ito
{
    int             stub;
}                   t_ito;

typedef struct      s_ito_specs
{
    char            *ip_address;
    int             pool;
}                   t_ito_specs;

typedef struct      s_package
{
	unsigned char   *mem;
	size_t		mem_cap;
	size_t          elem_count;
	size_t          index;
}                   t_package;

int     ito_validate(t_package *package, const char * restrict formatstr);
void    *ito_decompile_package(t_package *package, const char * restrict formatstr);
int     ito_compile_package(t_package *package, const char * restrict formatstr, ...);
int     ito_send_package(t_ito *ito, t_package *package);


t_ito   *ito_quick_init(char *ip);
t_ito	*ito_full_init(t_ito_specs *ito_specs);
int     ito_shutdown(t_ito *ito);
int		ito_hook(t_ito *ito, int (*f_ptr)(), void *param);

#endif

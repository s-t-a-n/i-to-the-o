/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/23 19:00:56 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/25 21:33:04 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITO_H
# define ITO_H

/*
** Include this header to use ITTO.
*/

int     ito_validate(t_package *package, const char * restrict formatstr);
void    *ito_decompile_package(t_package *package, const char * restrict formatstr);
int     ito_compile_package(t_package *package, const char * restrict formatstr, ...);
int     ito_send_package(t_ito *ito, t_package *package);


t_ito   *ito_quick_init(char *ip);
t_ito	*ito_full_init(t_ito_specs *ito_specs);
int     ito_shutdown(t_ito *ito);
int		ito_hook(t_ito *ito, int (*f_ptr)(), void *param);

#endif

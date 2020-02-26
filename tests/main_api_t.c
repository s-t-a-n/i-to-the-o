/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_api_t.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/26 20:34:29 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/26 23:11:16 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito.h"
#include "ito_internal.h"

int		main(int argc, char **argv)
{
	t_package package;

	ito_compile_package(&package, "%i", 5);

	return (0);
	argc = 0;
	argv = NULL;
}

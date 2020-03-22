/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   package.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 15:01:24 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 20:07:21 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ito_internal.h"
#include "networking/constants.h"
#include "packaging/packaging.h"

void		package_destroy(t_package *package)
{
	free(package->mem);
	free(package);
}

t_package	*package_initialise(void)
{
	t_package *package;

	package = malloc(sizeof(t_package));
	if (package)
	{
		package->mem = malloc(PACKAGE_DEF_SIZE + 1);
		if (package->mem)
		{
			package->mem_cap = PACKAGE_DEF_SIZE;
			package->index = 0;
		}
		else
		{
			free(package);
			handle_error("package_initialise", strerror(errno), NULL, ERR_CRIT);
		}	
	}
	else
		handle_error("package_initialise", strerror(errno), NULL, ERR_CRIT);
	return (package);
}

void		dump_package(t_package *package)
{
	size_t	index = 0;
	int		ctr = 0;

	printf("PACKAGE: --------------------\n");
	while (index < package->index)
	{
		printf("elem[%i] : |%s|\n", ctr, (char *)&package->mem[index]);
		index += strlen((char *)&package->mem[index]) + 1;
		ctr++;
	}
	printf("PACKAGE: --------------------\n");
}

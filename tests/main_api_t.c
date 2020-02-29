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

void		dump_package(t_package *package)
{
	size_t	index = 0;
	int	ctr = 0;
	while (index < package->index)
	{
		printf("elem[%i] : |%s|\n", ctr, (char *)&package->mem[index]);
		index += strlen((char *)&package->mem[index]) + 1;
		ctr++;
	}
}

int		main(int argc, char **argv)
{
	t_package package;

	ito_compile_package(&package, "%i%i%i%i%f%s%i%i%i%i", 232323, 3, 2, 1, 5.4299, "string string string", 1, 2 , 3, 19191);
	dump_package(&package);
	free(package.mem);

	return (0);
	argc = 0;
	argv = NULL;
}

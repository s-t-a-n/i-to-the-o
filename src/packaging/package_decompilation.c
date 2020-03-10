/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   package_decompilation.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/29 15:01:30 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 15:24:30 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

#include "ito_internal.h"
#include "packaging/packaging.h"

static int	int_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%i", (signed int *)var));
}

static int	uint_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%u", (unsigned int *)var));
}

static int	long_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%li", (signed long *)var));
}

static int	ulong_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%lu", (unsigned long *)var));
}

static int	long_long_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%lli", (signed long long *)var));
}

static int	ulong_long_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%llu", (unsigned long long *)var));
}

static int	double_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%lf", (double *)var));
}

static int	long_double_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%Lf", (long double *)var));
}

static int	get_data_of_type_from_package(int (*f_type_sscanf)(const char *, void *), const char *src, void *var, t_package *package)
{
	if (f_type_sscanf(src, var) == 1)
	{
		package->index += strnlen(src, package->mem_cap - package->index - 1) + 1;
		return (1);
	}
	else
		handle_error("get_data_of_type_from_package", "sscanf couldnt extract data", NULL, ERR_WARN);
	return (0);
}

int			get_data_from_package(va_list *args, const char * restrict formatstr, t_package *package)
{
	switch(*formatstr)
	{
		case 'i':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED INT from package");
			return (get_data_of_type_from_package(int_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, signed int *), package));
		case 'd':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED INT from package");
			return (get_data_of_type_from_package(int_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, signed int *), package));
		case 'u':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting UNSIGNED INT from package");
			return (get_data_of_type_from_package(uint_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, unsigned int *), package));
		case 'f':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting DOUBLE from package");
			return (get_data_of_type_from_package(double_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, double *), package));
		case 's':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting STRING from package");
			char **ptr = va_arg(*args, char **);
			*ptr = strndup((char *)&package->mem[package->index], package->mem_cap - package->index - 1);
			package->index += strnlen((char *)&package->mem[package->index], package->mem_cap - package->index - 1) + 1;
			return (*ptr != NULL);
		case 'l':
			if (*(formatstr + 1) == 'i')
			{
				LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED LONG from package");
				return (get_data_of_type_from_package(long_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, signed long), package));
			}
			else if (*(formatstr + 1) == 'u')
			{
				LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting UNSIGNED LONG from package");
				return (get_data_of_type_from_package(ulong_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, unsigned long *), package));
			}
			else if (*(formatstr + 1) == 'f')
			{
				LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting LONG DOUBLE from package");
				return (get_data_of_type_from_package(long_double_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, long double *), package));
			}
			else if (*(formatstr + 1) == 'l')
			{
				if (*(formatstr + 2) == 'i')
				{
					LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED LONG LONG from package");
					return (get_data_of_type_from_package(long_long_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, signed long long *), package));

				}
				else if (*(formatstr + 2) == 'u')
				{
					LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting UNSIGNED LONG LONG from package");
					return (get_data_of_type_from_package(ulong_long_sscanf, (char *)&package->mem[package->index], (void *)va_arg(*args, unsigned long long *), package));
				}
				else
					handle_error("get_data_from_package", "unknown conversion specifier", NULL, ERR_CRIT);
			}
			else
					handle_error("get_data_from_package", "unknown conversion specifier", NULL, ERR_CRIT);
			break;
		default:
			handle_error("get_data_from_package", "unknown conversion specifier", NULL, ERR_CRIT);
			return  (ITO_ERROR);
	}
	return  (ITO_ERROR);
}

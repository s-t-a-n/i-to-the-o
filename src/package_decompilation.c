/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   package_decompilation.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/29 15:01:30 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/29 17:29:32 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"

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

static int	double_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%lf", (double *)var));
}

static int	long_double_sscanf(const char *src, void *var)
{
	return(sscanf(src, "%Lf", (long double *)var));
}

static void	*get_data_for_type_from_package(int (*f_type_sscanf)(const char *, void *), const char *src, void *var, t_package *package)
{
			if (f_type_sscanf(src, var) == 1)
			{
				package->index += strnlen(src, package->mem_cap - package->index - 1) + 1;
				return (var);
			}
			else
				handle_error("get_data_for_type_from_package", "sscanf couldnt extract data", NULL, ERR_WARN);
			return (NULL);
}

void	*get_data_from_package(const char * restrict formatstr, t_package *package)
{
	int	(*f_type_sscanf)(const char *, void *);

	switch(*formatstr)
	{
		case 'i':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED INT from package");
			f_type_sscanf = int_sscanf;
			int var;
			return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var, package));
			break;
		case 'd':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED INT from package");
			f_type_sscanf = int_sscanf;
			int var1;
			return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var1, package));
			break;
		case 'u':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting UNSIGNED INT from package");
			f_type_sscanf = uint_sscanf;
			unsigned int var2;
			return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var2, package));
			break;
		case 'f':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting DOUBLE from package");
			f_type_sscanf = double_sscanf;
			double var3;
			return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var3, package));
			break;
		case 's':
			LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting STRING from package");
			char *var4;
			var4 = strndup((char *)&package->mem[package->index], package->mem_cap - package->index - 1);
			package->index += strnlen((char *)&package->mem[package->index], package->mem_cap - package->index - 1) + 1;
			return (void *)var4;
			break;
		case 'l':
			if (*(formatstr + 1) == 'i')
			{
				LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting SIGNED LONG from package");
			f_type_sscanf = long_sscanf;
			signed long var5;
			return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var5, package));
			}
			else if (*(formatstr + 1) == 'u')
			{
				LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting UNSIGNED LONG from package");
				f_type_sscanf = ulong_sscanf;
				unsigned long var6;
				return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var6, package));
			}
			else if (*(formatstr + 1) == 'f')
			{
				LOG_DEBUG("%s : %s\n","get_data_from_package", "extracting LONG DOUBLE from package");
				f_type_sscanf = long_double_sscanf;
				long double var7;
				return (get_data_for_type_from_package(f_type_sscanf, (char *)&package->mem[package->index], (void *)&var7, package));
			}
			break;
		default:
			handle_error("get_data_from_package", "unknown conversion specifier",
					NULL, ERR_CRIT);
			return  (NULL);
	}
	return  (NULL);
	package = NULL;
}

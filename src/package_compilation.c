/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   package_compilation.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/25 18:58:07 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/27 00:31:00 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"

static int	int_snprintf(char *dst, size_t size, void *var)
{
	return(snprintf(dst, size, "%i",*(int *)var));
}


static int	add_data_of_type_to_package(int (*f_type_snprintf)(char *, size_t, void *), void *var, t_package *package)
{
	int		len;
	unsigned char	*mem_p_cmp;

	len = f_type_snprintf(NULL, 0, var) + 1;
	if (len > 0)
	{
		if (package->index + len < package->mem_cap)
		{
			package->index += f_type_snprintf((char *)&package->mem[package->index], len, var) + 1;
			(package->elem_count)++;
		}
		else if ((package->mem_cap *= 2) < MEMCAP_MAX)
		{
			LOG_DEBUG("%s : %s\n","add_data_of_type_to_package", "reallocing package->mem");
			mem_p_cmp = (unsigned char *)realloc(package->mem, package->mem_cap);
			if (mem_p_cmp && mem_p_cmp != package->mem)
			{
				package->mem = mem_p_cmp;
				return(add_data_of_type_to_package(f_type_snprintf, var, package));
			}
			else
			{
				free(mem_p_cmp);
				handle_error("add_data_of_type_to_package", "realloc failure!", NULL, ERR_WARN);
			}
		}
		else
		{
			handle_error("add_data_of_type_to_package", "MEMCAP_MAX exceeded!", NULL, ERR_WARN);
			return (ITO_ERROR);
		}
	}
	else
	{
		handle_error("add_[data]_to_package", "zero data length!", NULL, ERR_WARN);
		return (ITO_ERROR);
	}
	return (ITO_SUCCESS);
}

int		add_data_to_package(va_list *args, const char * restrict formatstr,
		t_package *package)
{
	int	(*f_type_snprintf)(char *, size_t, void *);

	switch(*formatstr)
	{
		case 'i':
			LOG_DEBUG("%s : %s\n","add_data_to_package", "adding SIGNED INT to package");
			f_type_snprintf = &int_snprintf;
			int var = va_arg(*args, signed int);
			return(add_data_of_type_to_package(f_type_snprintf, (void *)&var, package));
			break;
		case 'd':
			LOG_DEBUG("%s : %s\n","add_data_to_package", "adding SIGNED INT to package");
			break;
		case 'u':
			LOG_DEBUG("%s : %s\n","add_data_to_package", "adding UNSIGNED INT to package");
			break;
		case 'f':
			LOG_DEBUG("%s : %s\n","add_data_to_package", "adding DOUBLE to package");
			break;
		case 's':
			LOG_DEBUG("%s : %s\n","add_data_to_package", "adding STRING to package");
			break;
		case 'l':
			if (*(formatstr + 1) == 'i')
			{
				LOG_DEBUG("%s : %s\n","add_data_to_package", "adding SIGNED LONG to package");
			}
			else if (*(formatstr + 1) == 'u')
			{
				LOG_DEBUG("%s : %s\n","add_data_to_package", "adding UNSIGNED LONG to package");

			}
			else if (*(formatstr + 1) == 'f')
			{
				LOG_DEBUG("%s : %s\n","add_data_to_package", "adding LONG DOUBLE to package");

			}
			break;
		default:
			handle_error("handle_conversion", "unknown conversion specifier",
					NULL, ERR_CRIT);
			return (ITO_ERROR);
	}
	return (ITO_ERROR);
}

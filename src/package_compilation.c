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

static int	conv_int_to_ucharstr(t_uchar_buffer *buffer, signed int var)
{
	if ((buffer->len = snprintf(NULL, 0, "%i", var)))
	{
		if ((buffer->mem = (unsigned char *)malloc(buffer->len + 1)))
		{
			if (snprintf((char *)buffer->mem, buffer->len, "%i", var) != buffer->len)
				free(buffer->mem);
			else
				return (0);
		}
	}
	return (1);
}

static int	add_buffer_to_package(t_uchar_buffer *buffer, t_package *package)
{
	if (package->mem_size + buffer->len < package->mem_cap)
	{
		return (1);
	}
	else
		return (0);
}

int		add_data_to_package(va_list *args, const char * restrict formatstr,
		t_package *package)
{
	t_uchar_buffer	buffer;
	int 			error;

	error = 0;
	switch(*formatstr)
	{
		case 'i':
			log_debug("%s : %s\n","add_data_to_package", "adding SIGNED INT to package");
			error |= conv_int_to_ucharstr(&buffer, va_arg(*args, signed int));
			break;
		case 'd':
			log_debug("%s : %s\n","add_data_to_package", "adding SIGNED INT to package");
			error |= conv_int_to_ucharstr(&buffer, va_arg(*args, signed int));
			break;
		case 'u':
			log_debug("%s : %s\n","add_data_to_package", "adding UNSIGNED INT to package");
			break;
		case 'f':
			log_debug("%s : %s\n","add_data_to_package", "adding DOUBLE to package");
			break;
		case 's':
			log_debug("%s : %s\n","add_data_to_package", "adding STRING to package");
			break;
		case 'c':
			log_debug("%s : %s\n","add_data_to_package", "adding CHAR to package");
			break;
		case 'l':
			if (*(formatstr + 1) == 'i')
			{
				log_debug("%s : %s\n","add_data_to_package", "adding SIGNED LONG to package");
			}
			else if (*(formatstr + 1) == 'u')
			{
				log_debug("%s : %s\n","add_data_to_package", "adding UNSIGNED LONG to package");

			}
			else if (*(formatstr + 1) == 'f')
			{
				log_debug("%s : %s\n","add_data_to_package", "adding LONG DOUBLE to package");

			}
			break;
		default:
			handle_error("handle_conversion", "unknown conversion specifier",
					NULL, ERR_CRIT);
			return (ITO_ERROR);
	}
	if (!error && add_buffer_to_package(&buffer, package))
		return (ITO_SUCCESS);
	else
		return (ITO_ERROR);
}

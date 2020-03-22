
#ifndef ERROR_H
# define ERROR_H

#include <string.h> /* for use of strerror() */

typedef enum	e_error
{
	ERR_WARN = 1,
	ERR_CRIT = 2
}				t_error;

typedef enum	e_stdfd
{
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2
}				t_stdfd;

void			handle_error(const char *function,
							const char *err_description,
							const char *extra,
							t_error errcode);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 14:36:12 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:27:31 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

typedef enum				e_request_type t_request_type;
typedef struct				s_queue t_queue;

typedef struct				s_client
{
	pthread_t				master;
	pthread_t				*workers;
	int						workers_count;
	t_queue					*queue;
	int						state;
}							t_client;

/*
** client.c
*/
t_client					*client_initialise(void);
void						client_shutdown(t_client *client);

#endif

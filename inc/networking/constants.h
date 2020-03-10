/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   constants.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 14:41:48 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 17:40:08 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

/*
** network defines
*/

# define NT_PORT			4200

# define NT_QUEUE_CAP		1024 // maximum number of pending requests
# define NT_QUEUE_BACKLOG	10

# define NT_WORKERS_DEF		5  // an extra thread is spawned for handling incoming
# define NT_WORKERS_MAX		20 // maximum number of threads to spin up

# define NT_RECV_BUFSIZE	1024
# define NT_SEND_BUFSIZE	1024

# define NT_CON_INIT		0x01
# define NT_CON_EXIT		0x02
# define NT_CON_PACK		0x04

# define NT_STATE_READY		1
# define NT_STATE_STOP		2

# define NT_WRITE_DELAY		50	// time in microseconds that the thread holds before attempting to write again
# define NT_READ_DELAY		50
# define NT_WRITE_MAXRETRY	10	// max amount of times write may come back zero
# define NT_READ_MAXRETRY	10


/*
** framing
*/

# define FRAME_HEADER_SIG				0x424242 // single integer id
# define FRAME_FOOTER_SIG				0x242424

# define FRAME_HEADER_SIG_LEN			4
# define FRAME_HEADER_REQTYPE_LEN		4
# define FRAME_HEADER_ROUTINGDATA_LEN	16 // 4 integer space of routing vars

# define FRAME_FOOTER_SIG_LEN			4

# define FRAME_HEADER_LEN				24 // combination of lengths above
# define FRAME_FOOTER_LEN				4

/*
** pooling
*/
# define POOL_RELAY_COUNT				5
# define POOL_MEMBR_COUNT				5

/*
** container
*/
# define CONTAINER_MEMCAP_DEF			512
# define CONTAINER_MEMCAP_MAX			2048


#endif

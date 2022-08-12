#include <netinet/udp.h> //UDP header
#include <netinet/ip6.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define UDP_PACKET_LENGTH sizeof(struct ip6_hdr) + sizeof(struct udphdr)

// A struct to keep the settings for each packet, both for source and target details:
struct packet_settings
{
	uint8_t protocol;
	char target_ip_address[39];
	char source_ip_address[39];
	__u_int target_port;
	__u_int source_port;
};

void get_random_ip(char *ip_buffer);
void get_random_port(__u_int *port);
void create_udp_header(char *packet, struct packet_settings *settings);
void create_packet(char *packet, struct packet_settings *settings);
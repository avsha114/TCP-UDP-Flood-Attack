#include <netinet/udp.h> //UDP header
#include <netinet/tcp.h> //TCP header
#include <netinet/ip.h>	 //IP header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define UDP_PACKET_LENGTH sizeof(struct iphdr) + sizeof(struct udphdr)
#define TCP_PACKET_LENGTH sizeof(struct iphdr) + sizeof(struct tcphdr)

// A struct to keep the settings for each packet, both for source and target details:
struct packet_settings
{
	uint8_t protocol;
	char target_ip_address[15];
	char source_ip_address[15];
	__u_int target_port;
	__u_int source_port;
};

// Psuedo headers for checksum calculations:
struct tcp_pseudo_header
{
	unsigned int source_address;
	unsigned int dest_address;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcp_length;

	struct tcphdr tcp;
};

struct udp_pseudo_header
{
	unsigned int source_address;
	unsigned int dest_address;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short udp_length;

	struct udphdr udp;
};

void get_random_ip(char *ip_buffer);
void get_random_port(__u_int *port);
void create_tcp_header(char *packet, struct packet_settings *settings);
void create_udp_header(char *packet, struct packet_settings *settings);
void create_packet(char *packet, struct packet_settings *settings);

unsigned short checksum(unsigned short *ptr, int nbytes);
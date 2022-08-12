#pragma once
#include <stdbool.h>
#include <unistd.h>
#include "packets.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 443

static const char options_help[] = "This program attacks a target with TCP RST flood. use -r to change attack type to UDP flood.\n"
                "Source IP and port will be randomized.\n\n"
                "How to use flood4: sudo ./flood4 [OPTIONS]\n"
                "\t-t: Target IP address. (Default = 127.0.0.1)\n"
                "\t-p: Target port. (Default = 443)\n"
                "\t-r: Toggle UDP flood Attack.\n";

void crash(const char *message);
bool is_number(const char* input, size_t length);
bool validate_port(const char *check);
bool validate_ip(const char *check);
void set_options(int argc, char *argv[], struct packet_settings *settings);

#include "options.h"

void crash(const char *message)
{
    printf("ERROR: %s\n", message);
    exit(1);
}

bool is_number(const char* input, size_t length)
{   
    // If one of the chars is not a digit return false:
    for (int i = 0; i < length ; i++)
    {
        if (!isdigit(input[i]))
            return false;
    }
    return true;
}

bool validate_port(const char *check)
{
    size_t length = strlen(check);

    if (length <= 0 || length > 5 || !is_number(check, length)) //5 is the maximum length of a port as a string. Max value is 65535
        crash("Port must be a number between 1 to 65535!");

    // Now we know 'check' is a number so we can convert it to unsigned int:
    __u_int target_port = atoi(check);
    if (target_port <= 0 || target_port > 65535)
        crash("Port must be a number between 1 to 65535!");

    return true;
}

bool validate_ip(const char *check)
{
    size_t length = strlen(check);
    struct in6_addr address;
    memset(&address, 0 ,sizeof(struct in6_addr));
    
    if(inet_pton(AF_INET6, check, &address) != 1)
        crash("IPv6 format is not valid!");

    // // strtok() modifies the string, so we need to copy 'check' to a temporary string
    // char test[15];
    // strncpy(test, check, 15);
    // char *token = strtok(test, "."); // Get the first 'word' before '.' was reached

    // int parts = 0; // Count how many parts we have. IPv4 Address should have 4 parts.
    
    // while(token != NULL)
    // {
    //     size_t part_len = strlen(token);

    //     // Check if current part is a number
    //     if (!is_number(token, part_len))
    //         crash("IP format is not valid!");
    //     int part = atoi(token);

    //     // IP address part can only be a number between 0 and 255.
    //     if (part >= 0 && part < 256)
    //     {
    //         parts++;
    //         token = strtok(NULL, "."); // Get next part
    //     }
    //     else
    //         crash("IP format is not valid!");
    // }

    // if (parts != 4)
    //     crash("IP format is not valid!");

    return true;
}

void set_options(int argc, char *argv[], struct packet_settings *settings)
{   
    //Initialize variables for input analyzing purposes:
    int option = 0;
    bool manual_ip = false, manual_port = false;
    
    // Initialize our options array and next option:
    char *options = "t:p:";
    option = getopt(argc, argv, options);

    while (option != -1)
    {        
        switch (option)
        {
        case 't':
            // Check that target IP is legal, If the program didn't crash - copy the argument to the settings struct:
            validate_ip((char*)optarg);
            strcpy(settings->target_ip_address, (char*)optarg);
            manual_ip = true; // Set for later
            break;
        case 'p':
            // Check that target port is legal, If the program didn't crash - copy the argument to the settings struct:
            validate_port((char*)optarg);
            settings->target_port = atoi((char*)optarg);
            manual_port = true;// Set for later
            break;

        default:
            fprintf( stderr, "%s", options_help);
            exit(1);
        }
        option = getopt(argc, argv, options); // Get next option
    }

    settings->protocol = IPPROTO_UDP ;
    
    // If no IP or port was set by the user, set default values accordingly:
    if (!manual_port)
        settings->target_port = DEFAULT_PORT;
    if (!manual_ip)
        strcpy(settings->target_ip_address, DEFAULT_IP);
}
#  TCP/UDP Flood Attackers

'flood4' and 'flood6' are tools for attacking a target with TCP-RST or UDP flood attacks.

Each packet is sent with a spoofed source IP address.

You can choose between IPv4 or IPv6 attacks.

These tools were written for learning purposes only. You must NOT use them for evil.

# Requiremnts

For Linux users - no requirements.

For Windows users - move to Linux :)

# Code Summary

The tools are each constructed with 3 source files (and 2 header files).

	-options.h/.c - used to configure the settings for the attack - IP, PORT, TCP/UDP.
                  
	-packets.h/.c - used to build packets including different headers and calculate checksum (on IPv4).
  
	-flood4.c/flood6.c - the main program and where the magic happens.
  
*The target IP address and port should be given by the user.*

*The source IP address and port are being randomized for each packet.*

# How to use:
For IPv4 attacks:
  1. Go to the "flood4" folder.
  2. Open a terminal window in that folder and type "make" .
  3. Use the program as follows:
  
	sudo ./flood4 [OPTIONS]

	OPTIONS: -t [IPv4 ADDRESS] - Use it to specify the target's IP address. (Default 127.0.0.1)
		 -p [PORT] - Use it to specify the target's port. (Default 443)
		 -r - Toggle UDP flood attack instead of the default TCP/RST.

For IPv6 attacks:
  1. Go to the "flood6" folder.
  2. Open a terminal window in that folder and type "make" .
  3. Use the program as follows:
  
	sudo ./flood6 [OPTIONS]

	OPTIONS: -t [IPv6 ADDRESS] - use it to specify the target's IP address. (Default ::1)
		 -p [PORT] - use it to specify the target's port. (Default 443)

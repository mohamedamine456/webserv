#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include "../servers/Socket.hpp"
#define MAX_SERVERS 1
#define PORT 8000

std::vector< std::string >		StringSplit( std::string str, std::string delimiter );
std::vector< Socket >			create_multiple_servers();
std::string						randomfilename( std::string str );

#endif
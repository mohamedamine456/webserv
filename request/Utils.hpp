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
#define RECV_SIZE 4

std::vector< std::string >		StringSplit( std::string str, std::string delimiter );
std::vector< Socket >			create_multiple_servers();
std::string						getfilename( std::string str );

struct RequestParse {
	std::string		filename;
	char			buffer[RECV_SIZE];
	std::string		requestLine;
	bool			rlf;
	std::string		headers;
	bool			hf;
	std::ofstream	rqstFile;
	size_t			totalread;

	RequestParse() {
		requestLine = "";
		headers = "";
		filename = "/var/tmp/request_" + getfilename("");
		rqstFile.open(filename, std::ofstream::out);
		rlf = false;
		hf = false;
		totalread = 0;
	}
};

void	add_buffer( RequestParse &parser );
void	check_requestLine( RequestParse &parser );
void	check_headers( RequestParse &parser );
void	read_content_length( RequestParse &parser, int &newSockfd );
void    read_chunked( RequestParse &parser, int &newSockfd );

#endif
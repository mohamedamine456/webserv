#include "Utils.hpp"

void	add_buffer( RequestParse &parser ) {
	if (parser.rlf == false) {
		parser.requestLine += std::string(parser.buffer);
	}
	else if (parser.hf == false) {
		parser.headers += std::string(parser.buffer);
	}
	else {
		parser.rqstFile << parser.buffer;
	}
}

void	check_requestLine( RequestParse &parser ) {
	if (parser.rlf == false) {
		if (parser.requestLine.find("\r\n") != std::string::npos)
		{
			parser.headers = parser.requestLine.substr(parser.requestLine.find("\r\n") + 2);
			parser.requestLine = parser.requestLine.substr(0, parser.requestLine.find("\r\n"));
			parser.rlf = true;
		}
	}
}

void	check_headers( RequestParse &parser ) {
	if (parser.hf == false) {
		if (parser.headers.find("\r\n\r\n") != std::string::npos)
		{
			std::string rest = parser.headers.substr(parser.headers.find("\r\n\r\n") + 4);
			parser.headers = parser.headers.substr(0, parser.headers.find("\r\n\r\n"));
			parser.rqstFile << rest;
            parser.totalread += rest.length();
			parser.hf = true;
		}
	}
}

void	read_content_length( RequestParse &parser, int &newSockfd ) {
    int				recvLength;
    size_t          content_length = stoi(parser.headers.substr(parser.headers.find("Content-Length:") + 16));
    while ((recvLength = recv(newSockfd, &parser.buffer, RECV_SIZE, 0))) {
        parser.buffer[recvLength] = '\0';
		parser.totalread += recvLength;
        parser.rqstFile << parser.buffer;
        if (parser.totalread >= content_length)
            break ;
    }
}

void    read_chunked( RequestParse &parser, int &newSockfd ) {
    // int				recvLength;
    // while ((recvLength = recv(newSockfd, &parser.buffer, RECV_SIZE, 0))) {
    //     parser.buffer[recvLength] = '\0';
	// 	parser.totalread += recvLength;
    //     parser.rqstFile << parser.buffer;
    //     if ()
    // }
}
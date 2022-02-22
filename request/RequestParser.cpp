#include "Utils.hpp"

void	add_buffer( RequestParse &parser, int &recvLength ) {
    if (parser.rlf == false) {
        parser.requestLine.resize(parser.requestLine.length() + recvLength);
        parser.requestLine.append(parser.buffer, recvLength);
    }
    else if (parser.hf == false) {
        parser.headers.resize(parser.headers.length() + recvLength);
        parser.headers.append(parser.buffer, recvLength);
    }
    else {
        parser.rqstFile.write(parser.buffer, recvLength);
    }
}

void	check_requestLine( RequestParse &parser ) {
    int     found;
    if (parser.rlf == false) {
        if ((found = parser.requestLine.find("\r\n")) != std::string::npos)
        {
            parser.headers.resize(parser.headers.length() + parser.requestLine.length() - found - 2);
            parser.headers.append(parser.requestLine, found + 2, parser.requestLine.length() - found - 2);
            parser.requestLine.resize(found);
            parser.rlf = true;
        }
    }
}

void	check_headers( RequestParse &parser ) {
    int         found;
    if (parser.hf == false) {
        if ((found = parser.headers.find("\r\n\r\n")) != std::string::npos)
        {
            parser.rqstFile.write(parser.headers.c_str() + found + 4, parser.headers.length() - found - 4);
            parser.totalread += parser.headers.length() - found - 4;
            parser.headers.resize(found);
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
        parser.rqstFile.write(parser.buffer, recvLength);
        if (parser.totalread >= content_length)
            break ;
    }
}

void    read_chunked( RequestParse &parser, int &newSockfd ) {
    std::string     combin;
    int				recvLength;
    while ((recvLength = recv(newSockfd, &parser.buffer, RECV_SIZE, 0))) {
        parser.buffer[recvLength] = '\0';
		parser.totalread += recvLength;
        parser.rqstFile.write(parser.buffer, recvLength);
        if (std::string(parser.buffer).find("\r\n\r\n") != std::string::npos)
            break;
        else if (combin != "") {
            combin += std::string(parser.buffer, 0, recvLength);
            if (std::string(parser.buffer).find("\r\n\r\n") != std::string::npos) {
                break ;
            } else {
                combin = combin.substr(combin.length() - 5);
            }
        }
        else {
            combin = std::string(parser.buffer, recvLength - 5, 5);
        }
    }
}
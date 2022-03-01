#include "Request.hpp"

Request::Request (): _error(false) {
	this->_bodyfilename = "/var/tmp/request_" + randomfilename("") + "_body";
}

Request::Request (const Request &rqst ) {
	*this = rqst;
}

Request::~Request () {}

Request	&Request::operator= ( const Request &rqst ) {
	this->_method = rqst._method;
	this->_path = rqst._path;
	this->_query_string = rqst._query_string;
	this->_version = rqst._version;
	this->_host = rqst._host;
	this->_port = rqst._port;
	this->_headers = rqst._headers;
	this->_bodyfilename = rqst._bodyfilename;
	this->_error = rqst._error;

	return *this;
}

// add Buffer to right place
int		Request::add_buffer( int &recvLength, char *buffer ) {
	std::string	bufferString(buffer, recvLength);
	if (this->_rqstLexer.getLineSet() == false) {
		add_request_line(bufferString);
	}
	if (this->_rqstLexer.getHeadersSet() == false && this->_rqstLexer.getLineSet() == true) {
		add_headers(bufferString);
	}
	if (this->_rqstLexer.getLineSet() == true && this->_rqstLexer.getHeadersSet() == true) {
		// here choose how to write in file
		// if (this->_fileOpened == false)
		// 	this->_bodyFile.open(this->_bodyfilename, std::ofstream::out);
		// this->_bodyFile.write(buffer, recvLength);
	}
	return false;
}

void	Request::add_request_line( std::string &buffer ) {
	size_t	found;
	if ((found = buffer.find("\r\n")) != std::string::npos)
	{
		this->_rqstLexer.getRequestLine() += buffer.substr(0, found);
		buffer = buffer.substr(found + 2);
		this->_rqstLexer.setLineSet();
	}
	else {
		this->_rqstLexer.getRequestLine() += buffer;
	}
}

void	Request::add_headers( std::string &buffer ) {
	size_t	found;
	if ((found = buffer.find("\r\n\r\n")) != std::string::npos)
	{
		this->_rqstLexer.getHeaders() += buffer.substr(0, found);
		buffer = buffer.substr(found + 4);
		this->_rqstLexer.setHeadersSet();
	}
	else {
		this->_rqstLexer.getHeaders() += buffer;
	}
}

// void	Request::read_content_length( int &newSockfd ) {
// 	int				recvLength;
// 	size_t          content_length = stoi(this->_headers.substr(this->_headers.find("Content-Length:") + 16));
// 	if (this->_fileOpened == false)
// 		this->_rqstFile.open(this->_filename, std::ofstream::out);
// 	while ((recvLength = recv(newSockfd, &this->buffer, RECV_SIZE, 0))) {
// 		this->buffer[recvLength] = '\0';
// 		this->_totalread += recvLength;
// 		this->_rqstFile.write(this->buffer, recvLength);
// 		if (this->_totalread >= content_length)
// 			break ;
// 	}
// }

// void    Request::read_chunked( int &newSockfd ) {
// 	std::string     combin;
// 	int				recvLength;
	
// 	if (this->_fileOpened == false)
// 		this->_rqstFile.open(this->_filename, std::ofstream::out);
// 	while ((recvLength = recv(newSockfd, &this->buffer, RECV_SIZE, 0))) {
// 		this->buffer[recvLength] = '\0';
// 		this->_totalread += recvLength;
// 		this->_rqstFile.write(this->buffer, recvLength);
// 		if (std::string(this->buffer).find("\r\n\r\n") != std::string::npos)
// 			break;
// 		else if (combin != "") {
// 			combin += std::string(this->buffer, 0, recvLength);
// 			if (std::string(this->buffer).find("\r\n\r\n") != std::string::npos) {
// 				break ;
// 			} else {
// 				combin = combin.substr(combin.length() - 5);
// 			}
// 		}
// 		else {
// 			combin = std::string(this->buffer, recvLength - 5, 5);
// 		}
// 	}
// }

// Setters
void	Request::setHeaders( std::vector< std::string > &headers ) {
	for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
		std::string key = (*it).substr(0, (*it).find(':'));
		std::string value = (*it).substr((*it).find(':') + 1);
		trimString(value, ' ');
		this->_headers.push_back(std::make_pair(key, value));
	}
}

void		Request::setMethod ( std::string &firstLine ) {
	if (firstLine.find(' ') != std::string::npos) {
		this->_method = firstLine.substr(0, firstLine.find(' '));;
		// test for supported methods
		firstLine.erase(0, firstLine.find(' ') + 1);
	}
	else {
		this->_error = true;
	}
}

void		Request::setPath ( std::string &firstLine ) {
	if (firstLine.find("?") != std::string::npos) {
		this->_query_string = firstLine.substr(0, firstLine.find("?"));
		firstLine.erase(firstLine.find("?"));
	}
}

void		Request::setQueryString ( std::string &firstLine ) {
	if (firstLine.find("?") != std::string::npos) {
		this->_query_string = firstLine.substr(firstLine.find("?") + 1);
		firstLine.erase(firstLine.find("?") + 1);
	}
}

void		Request::setVersion ( std::string &firstLine ) {
	if (firstLine.length() > 0) {
		this->_version = firstLine;
	}
	else {
		this->_error = true;
	}
}

void		Request::setHostHeaders() {
	for (std::vector< std::pair<std::string, std::string> >::iterator it = this->_headers.begin();
	it != this->_headers.end(); it++) {
		if (it->first == "Host") {
			setHost(it->second);
			break;
		}
	}
}

void		Request::setHost ( std::string hostString ) {
	if (hostString.find(':') != std::string::npos) {
		std::string tmpPort = hostString.substr(hostString.find(':') + 1);
		this->setPort(tmpPort);
		this->_host = hostString.substr(0, hostString.find(':'));
	}
	else {
		this->_host = hostString;
		this->setPort("");
	}
}

void		Request::setPort ( std::string  portString ) {
	if (portString != "") {
		this->_port = std::stoi(portString);
	}
	else {
		this->_port = 80;
	}
}

void		Request::addHeader ( std::string header ) {
	std::string key = header.substr(0, header.find(":"));
	std::string value = header.substr(header.find(":") + 1);
	trimString(value, ' ');
	this->_headers.push_back(std::make_pair(key, value));
}

void		Request::setBodyfile ( std::string filename ) {
	this->_bodyfilename = filename;
}

// Getters

RequestLexer	&Request::getRequestLexer() {
	return this->_rqstLexer;
}

std::string		&Request::getMethod () {
	return this->_method;
}

std::string		&Request::getPath () {
	return this->_path;
}

std::string		&Request::getQueryString () {
	return this->_query_string;
}

std::string		&Request::getVersion () {
	return this->_version;
}

std::string		&Request::getHost () {
	return this->_host;
}

int				&Request::getPort () {
	return this->_port;
}

std::vector< std::pair<std::string, std::string> >	&Request::getHeaders () {
	return this->_headers;
}

std::string		&Request::getBodyfilename () {
	return this->_bodyfilename;
}

size_t			&Request::getTotalread() {
	return this->_totalread;
}
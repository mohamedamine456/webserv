#include "Request.hpp"

Request::Request (): error(false) {}

Request::Request (const Request &rqst ) {
	*this = rqst;
}

Request::~Request () {}

Request	&Request::operator= ( const Request &rqst ) {
	this->method = rqst.method;
	this->path = rqst.path;
	this->query_string = rqst.query_string;
	this->version = rqst.version;
	this->host = rqst.host;
	this->port = rqst.port;
	this->headers = rqst.headers;
	this->body = rqst.body;
	this->error = rqst.error;

	return *this;
}

void	Request::setRequest(std::string &request) {
	
}

void		Request::setMethod ( std::string &firstLine ) {
	if (firstLine.find(' ') != std::string::npos) {
		this->method = firstLine.substr(0, firstLine.find(' '));
		// test for supported methods
		firstLine.erase(0, firstLine.find(' ') + 1);
	}
	else {
		this->error = true;
	}
}

void		Request::setPath ( std::string &firstLine ) {
	if (firstLine.find(' ') != std::string::npos) {
		this->path = firstLine.substr(0, firstLine.find(' '));
		// test for supported methods
		firstLine.erase(0, firstLine.find(' ') + 1);
	}
	else {
		this->error = true;
	}
}

void		Request::setQuery ( std::string &firstLine ) {

}

void		Request::setVersion ( std::string &firstLine ) {
	if (firstLine.length() > 0) {
		this->version = firstLine;
	}
	else {
		this->error = true;
	}
}

void		Request::setHost ( std::string &hostString ) {

}

void		Request::setPort ( std::string  &portString ) {

}

void		Request::setHeaders ( std::vector<std::string> &headers ) {

}

void		Request::setBody ( std::vector <std::string> &body ) {

}

std::string		Request::getMethod () {
	return this->method;
}

std::string		Request::getPath () {
	return this->path;
}

std::string		Request::getVersion () {
	return this->version;
}

// std::vector<std::pair<std::string, std::string> >	Request::splitRequest( std::string request ) {
	
// }
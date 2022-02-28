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
	this->bodyfilename = rqst.bodyfilename;
	this->error = rqst.error;

	return *this;
}

void	Request::setHeaders( std::vector< std::string > &headers ) {
	for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
		std::string key = (*it).substr(0, (*it).find(':'));
		std::string value = (*it).substr((*it).find(':') + 1);
		trimString(value, ' ');
		this->headers.push_back(std::make_pair(key, value));
	}
}

void		Request::setMethod ( std::string &firstLine ) {
	if (firstLine.find(' ') != std::string::npos) {
		this->method = this->method = firstLine.substr(0, firstLine.find(' '));;
		// test for supported methods
		firstLine.erase(0, firstLine.find(' ') + 1);
	}
	else {
		this->error = true;
	}
}

void		Request::setPath () {
	
}

void		Request::setQuery ( std::string &firstLine ) {
	if (firstLine.find("?") != std::string::npos) {
		this->query_string = firstLine.substr(firstLine.find("?"));
		firstLine.erase(firstLine.find("?"));
	}
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
	if (hostString.find(':') != std::string::npos) {
		std::string tmpPort = hostString.substr(hostString.find(':') + 1);
		this->setPort(tmpPort);
		this->host = hostString.substr(0, hostString.find(':'));
	}
	else {
		this->host = hostString;
		this->setPort("");
	}
}

void		Request::setPort ( std::string  portString ) {
	if (portString != "") {
		this->port = std::stoi(portString);
	}
	else {
		this->port = 80;
	}
}

void		Request::addHeader ( std::string header ) {
	std::string key = header.substr(0, header.find(":"));
	std::string value = header.substr(header.find(":") + 1);
	trimString(value, ' ');
	this->headers.push_back(std::make_pair(key, value));
}

void		Request::setBodyfile ( std::string filename ) {
	this->bodyfilename = filename;
}



std::string		&Request::getMethod () {
	return this->method;
}

std::string		&Request::getPath () {
	return this->path;
}

std::string		&Request::getVersion () {
	return this->version;
}

std::string		&Request::getHost () {
	return this->host;
}

int				&Request::getPort () {
	return this->port;
}

std::string		&Request::getBodyfile () {
	return this->bodyfilename;
}
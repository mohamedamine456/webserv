#include "Request.hpp"
#include "Utils.hpp"
#include <sys/socket.h>
#define RECV_SIZE 4096

std::string getfilename(std::string str) {
	static int a = 1;
	time_t ttime = std::time(0);
	std::string filename(std::to_string(ttime));
	filename.insert(filename.length(), std::to_string(a));
	a++;
	return (filename);
}

std::string	read_request(int newSockfd) {
	int					recvLength = RECV_SIZE;
	char				buffer[RECV_SIZE];
	std::string			filename = "/var/tmp/request_" + getfilename("");
	std::ofstream		rqstFile(filename, std::ofstream::out);
	while ((recvLength = recv(newSockfd, &buffer, RECV_SIZE, 0)) == RECV_SIZE) {
		buffer[recvLength] = '\0';
		rqstFile << buffer;
	}
	buffer[recvLength] = '\0';
	rqstFile << buffer;
	return (filename);
}

int main() {
	Request rqst;
	int							newSockfd = 3;		// should be connection socket fd
	std::vector< std::string >	parts;
	std::string					rqstFilename;
	std::string					bodyFilename;
	std::ofstream				bodyFile;
	std::ifstream				requestFile;
	std::string					rqstLine;

	rqstFilename = read_request(newSockfd);
	requestFile.open(rqstFilename, std::ifstream::in);
	if (requestFile.fail()) {
		std::cerr << "Open File Failed!" << std::endl;
		exit(EXIT_FAILURE);
	}
	getline(requestFile, rqstLine);
	if (rqstLine.empty()) {
		std::cerr << "Bad Request!" << std::endl;
		exit(EXIT_FAILURE);
	}

	while (getline(requestFile, rqstLine)) {
		if (rqstLine == "") {
			break ;
		}
		else {
			rqst.addHeader(rqstLine);
		}
	}
	while (getline(requestFile, rqstLine)) {
		bodyFile << rqstLine << std::endl;
	}
	rqst.setBodyfile("");
	return (0);
}
#include "../include/webserv.hpp"
#include "SocketInfos.hpp"

int		create_servers( std::vector< Server > &servers ) {
	std::vector<int>    bound;
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		// Create Socket
		if ((*it).get_socketInfos().createSocket() == -1) {
			perror("Socket");
			return (-1);
		}

		// set Socket Address
		(*it).get_socketInfos().setSocketAddress((*it).get_port());

		// test if the socket is already bound to prevent bind failure
		if (find(bound.begin(), bound.end(), (*it).get_port()) == bound.end()) {
			// Bind Socket if not already bound
			if((*it).get_socketInfos().bindSocket() == -1) {
				perror("Bind");
				return (-1);
			}
		}

		// Listen on socket
		if ((*it).get_socketInfos().listenSocket() == -1) {
			perror("Listen");
			return (-1);
		}
	}
	return (0);
}

void	addFds( std::vector<Server> &servers, fd_set &rfds, int &maxfd ) {
	// to remove all file descriptors from the the set rfds
	FD_ZERO(&rfds);

	// iterate all servers and add file descriptor to rfds set with FD_SET
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
		FD_SET((*it).get_socketInfos().getSocketFd(), &rfds);

		// test for max fd
		if ((*it).get_socketInfos().getSocketFd() > maxfd)
			maxfd = (*it).get_socketInfos().getSocketFd();
	}
}

int		ready_fd( std::vector<Server> &servers, fd_set &rset ) {

	int fd = -1;
	// loop over all fds after select said the a fd is ready for read
	// to see which fd is ready for read using FD_ISSET
	for (unsigned int i = 0; i < servers.size(); i++) {
		if (FD_ISSET(servers[i].get_socketInfos().getSocketFd(), &rset)) {
			fd = servers[i].get_socketInfos().getSocketFd();
			break ;
		}
	}
	return (fd);
}

std::string getfilename() {
	static int a = 1;
	time_t ttime = std::time(0);
	std::string filename(std::to_string(ttime));
	filename.insert(filename.length(), std::to_string(a));
	a++;
	return (filename);
}

// read the request need improvements
Request	read_request(int &newSockfd) {
	Request				rqst;
	int					recvLength = 1024;								// length received in request
	char				buffer[1024];									// request reading buffer
	std::string			filename = "/var/tmp/request_" + getfilename();
	std::ofstream		rqstFile(filename, std::ofstream::out);
	std::cout << "Receiving:" << std::endl;
	while ((recvLength = recv(newSockfd, &buffer, 1024, 0)) == 1024) {
		buffer[recvLength] = '\0';
		rqstFile << buffer;
	}
	buffer[recvLength] = '\0';
	rqstFile << buffer;
	std::cout << filename << "\n";
	remove(filename.c_str());
	return rqst;
}

// to send simple ressponse
void	send_simple_response(int &newSockfd)
{
	std::string str_send = "HTTP/1.1 200 OK\nServer: Test Server\nContent-Type: text/plain\nContent-Length: 7\n\nHello!\n";
	send(newSockfd, str_send.c_str(), strlen(str_send.c_str()), 0);
}

// handle request and send a simple response
void	handle_request(int newSockfd)
{
	Request rqst = read_request(newSockfd);						// read request
	send_simple_response(newSockfd);							// to prevent multi request from mozilla
	std::cout << "End Reading!" << std::endl;
}

void	handle_all_servers( std::vector<Server> &servers, fd_set &rfds, int &maxfd ) {
	int					newSockfd;										// new connection FD										// server configuration
	struct sockaddr_in	connAddress;
	socklen_t stor_size = sizeof(struct sockaddr_in);
	
	// create another set bcs select is destroys the set feeded
	fd_set	rset;
	int		fd;
	unsigned int status;

	while (true) {
		// initialize fd set
		rset = rfds;

		// feed fds to select only for read option
		status = select(maxfd + 1, &rset, NULL, NULL, NULL);
		// protection for select
		if (status < 0) {
			std::cerr << "Select Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		if ((fd = ready_fd(servers, rset)) != -1)
		{
			// accept connection and create socket for the connection
			newSockfd = accept(fd, (struct sockaddr *)&connAddress, &stor_size);
			// protection for accept
			if (newSockfd < 0) {
				std::cerr << "Accepting Connection Failed!" << std::endl;
				exit(EXIT_FAILURE);
			}
			fcntl(newSockfd, F_SETFL, O_NONBLOCK);
			handle_request(newSockfd);

			// close connection socket after sending a response 
			close(newSockfd);
		}
	}
}
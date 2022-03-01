#include "servers.hpp"

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

void	add_servers( std::vector<Server> &servers, fd_set &rfds, int &maxfd ) {
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

int		servers_fd( std::vector<Server> &servers, fd_set &rset ) {

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

void	accept_connection( std::vector< std::pair< Client, Request > > &clients, int &fd )
{
	// New Connection configuration
	socklen_t stor_size = sizeof(struct sockaddr_in);
	Client clt;
	Request	rqst;

	clt.getClientFd() = accept(fd, (struct sockaddr *)&(clt.getClientAddress()), &stor_size);
	// protection for accept
	if (clt.getClientFd() < 0) {
		std::cerr << "Accepting Connection Failed!" << std::endl;
		exit(EXIT_FAILURE);
	}
	// fcntl(newSockfd, F_SETFL, O_NONBLOCK);		// check if this is the right place
	clients.push_back(std::make_pair< Client, Request >(clt, rqst));
}

void	send_simple_response(int &newSockfd)
{
	std::string str_send = "HTTP/1.1 200 OK\nServer: Test Server\nContent-Type: text/plain\nContent-Length: 7\n\nHello!\n";
	send(newSockfd, str_send.c_str(), strlen(str_send.c_str()), 0);
}

void	handle_all_servers( std::vector<Server> &servers, fd_set &read_fds, int &maxfd ) {
	char										buffer[RECV_SIZE + 1];	// buffer for read
	int											recvLength = 0;
	std::vector< std::pair< Client, Request > >	read_clients;
	std::vector< std::pair< Client, Request > >	write_clients;

	// create another set bcs select is destroys the set feeded
	fd_set	backup_rset;
	int		fd;
	unsigned int status;

	while (true) {
		// initialize fd set
		backup_rset = read_fds;

		// add client socket file descriptors to rset
		
		// feed fds to select only for read option
		status = select(maxfd + 1, &backup_rset, NULL, NULL, NULL);
		// protection for select
		if (status < 0) {
			std::cerr << "Select Failed!" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		if ((fd = servers_fd(servers, backup_rset)) != -1)
		{
			// accept connection and create socket for the connection
			accept_connection(read_clients, fd);
		}
		for(std::vector< std::pair< Client, Request > >::iterator it = read_clients.begin(); it != read_clients.end(); it++) {
			if (FD_ISSET(it->first.getClientFd(), &backup_rset)) {
				// read_request and add it to the propre on
				memset(buffer, '\0', RECV_SIZE);
				if (recvLength = recv(it->first.getClientFd(), buffer, RECV_SIZE, 0) == -1) {
					std::cerr << "Couldn't Read!" <<std::endl;
				}
				else {
					// if the request is finished add fd to writing list
					if (it->second.add_buffer(recvLength, buffer) == true) {

					}
				}
			}
		}
		for(std::vector< std::pair< Client, Request > >::iterator it = write_clients.begin(); it != write_clients.end(); it++) {
			if (FD_ISSET(it->first.getClientFd(), &backup_rset)) {
				// write part from response
				
				
				// if the response is finished remove the fd if connection not to keep alive
				
			}
		}
	}
}
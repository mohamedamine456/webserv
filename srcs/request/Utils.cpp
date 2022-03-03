#include "Utils.hpp"

// std::vector< SocketInfos >		create_multiple_servers()
// {
// 	std::vector<SocketInfos>		servers;
// 	for (int i = 0; i < MAX_SERVERS; i++)
// 	{
// 		SocketInfos	serv;
		
// 		// creation of socket
// 		if (serv.createSocket() < 0) {
// 			std::cerr << "Socket Creation Failed!" << std::endl;
// 			exit(EXIT_FAILURE);
// 		}

// 		// initialize address
// 		serv.setSocketAddress(PORT + (i * 100));

// 		if (serv.bindSocket() == -1) {
// 			std::cerr << "Bind Failed!" << std::endl;
// 			exit(EXIT_FAILURE);
// 		}

// 		// listen on socket
// 		if (serv.listenSocket() == -1) {
// 			std::cerr << "Listen Failed!" << std::endl;
// 			exit(EXIT_FAILURE);
// 		}
// 		servers.push_back(serv);
// 	}
// 	return servers;
// }

std::string randomfilename( std::string str ) {
	static int a = 1;
	time_t ttime = std::time(0);
	std::string filename(std::to_string(ttime));
	filename.insert(filename.length(), std::to_string(a) + "_" + str);
	a++;
	return (filename);
}

std::string		trimString( std::string str, char c )
{
	for (std::string::iterator it = str.begin(); *it == c; it++) {
        str.erase(it);
        it = str.begin();
    }
    if (*(str.begin()) == c)
        str.erase(str.begin());
    for (std::string::iterator it = str.end() - 1; *it == c; it--) {
        str.erase(it);
    }
	return (str);
}

std::vector< std::string >    StringSplit( std::string str, std::string delimiter ) {
	std::vector<std::string> parts;
	while (str.length() && str.find(delimiter) != std::string::npos) {
		std::string tmp = str.substr(0, str.find(delimiter));
			parts.push_back(tmp);
		str.erase(0, str.find(delimiter) + delimiter.length());
	}
	parts.push_back(str);
	return parts;
}
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

int main()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo("www.example.net", "3490", &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddrinfo Failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(servinfo);
    return (0);
}
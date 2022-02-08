#include "Request.hpp"
#include "Utils.hpp"

int main() {
    Request rqst;
    std::vector< std::string > parts;
    std::string rqstLine = "GET http://localhost:80/index.html HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:94.0) Gecko/20100101 Firefox/94.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nUpgrade-Insecure-Requests: 1\nSec-Fetch-Dest: document\nSec-Fetch-Mode: navigate\nSec-Fetch-Site: none\nSec-Fetch-User: ?1";
    parts = ft_split(rqstLine, "\n");
    rqst.setMethod(parts[0]);
    rqst.setPath(parts[0]);
    rqst.setVersion(parts[0]);

    std::cout << "Method: " << rqst.getMethod() << ", Path: " << rqst.getPath() << ", Version: " << rqst.getVersion() << std::endl;
    return (0);
}
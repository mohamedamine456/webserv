#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include "Utils.hpp"

class Request {
    private:
        typedef std::pair<std::string, std::string>		string_pair;
		RequestLexer									_rqstLexer;
		std::string										_method;
		std::string										_path;
		std::string										_query_string;
		std::string										_version;
		std::string										_host;
		int												_port;
        std::vector<string_pair>						_headers;
		std::string										_bodyfilename;
		std::ofstream									_bodyFile;
		bool											_fileOpened;
		size_t											_totalread;
		bool											_error;

		// private methods
	public:
		Request ();
		Request ( const Request &rqst );
		~Request ();
		Request											&operator= ( const Request &rqst );
		// add Buffer to right place
		int												add_buffer ( int &recvLength, char *add_buffer );
		void											add_request_line ( std::string &buffer );
		void											add_headers ( std::string &buffer );
		// Setters
		void											setMethod ( std::string &firstLine );
		void											setPath ( std::string &firstLine );
		void											setQueryString ( std::string &firstLine );
		void											setVersion ( std::string &firstLine );
		void											setHostHeaders();
		void											setHost ( std::string hostString );
		void											setPort ( std::string  portString );
		void											addHeader ( std::string header );
		void											setHeaders ( std::vector<std::string> &headers );
		void											setBodyfile ( std::string filename );
	
		
		// Getters
		RequestLexer									&getRequestLexer();
		std::string										&getMethod ();
		std::string										&getPath ();
		std::string										&getQueryString ();
		std::string										&getVersion ();
		std::string										&getHost ();
		int												&getPort ();
		std::vector<string_pair>						&getHeaders ();
		std::string										&getBodyfilename ();
		size_t											&getTotalread();
};

#endif
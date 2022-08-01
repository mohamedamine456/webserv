/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhebbat <yhebbat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 02:38:21 by yhebbat           #+#    #+#             */
/*   Updated: 2022/05/01 02:38:22 by yhebbat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "server.hpp"
#include <iostream>
#include <vector>
#include "../../include/webserv.hpp"


class CGI
{
protected:
    std::string                     _extention;
    std::string                     _cgi_path;
    std::vector<std::string>        _methods;
    char**                          _envp;
    char**                          _argv;
    std::string                     _cgi_response_file;
    std::string                     _status;
    std::string                     _header;
    // std::string                  _name;
public:
    CGI();
    ~CGI();

    // ACHRAAFFFF
    std::string                     header_to_envp(std::pair<std::string, std::string>);
    std::string                     generate_uri(Request request);
    void                            set_header(Response & response);
    void                            set_envp(Request request, Response response);
    std::string                     set_cgi_path( void );
    void                            set_argv(Response response);
    std::string                     set_cgi_response_file( void );
    std::string                     init_data();
    size_t                          getAccessType(std::string PATH);
    std::string                     cgi_process( Response & response, Request request );
	std::string					    set_status( void );

    std::string                     &get_extention( void );
    std::string                     &get_cgi_response_file( void );
    std::string                     &get_cgi_path( void );
    std::vector<std::string>        &get_methods( void );
    std::string                     get_script_name(Response response);
    std::string                     get_path_info(Response response);
    // Methods:
    std::string                     execute_cgi(Request request, Response & response);
    void                            fillResponseBuffer( Response & response );

    // YASSINNE
    std::string                     get_cgi_path() const;
    std::string                     get_cgi_methods(int i) const;
    std::string                     get_cgi_name() const;
    std::vector<std::string>        get_cgi_methods() const;
    void                            set_cgi_path(std::string cgi_path);
    void                            set_cgi_methods(std::string methods);
    void                            set_cgi_name(std::string name);
    unsigned int                    get_cgi_methods_size();
    bool                            not_predefined(std::string &word) const;
    cgi                             &operator=(cgi const & rhs);
};


#endif
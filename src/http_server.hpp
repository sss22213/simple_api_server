#pragma once
#include <thread>
#include <vector>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <regex>
using namespace std;

// response
static std::string _200_response = "HTTP/1.1 200 OK\r\n";
static std::string _400_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n";
static std::string _403_response = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n";
static std::string _404_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
static std::string _405_response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n\r\n";
namespace http_server{
    std::vector<std::string> routing_analysis(std::string);
    /*
    HTTP/1.1 response_status
    Content-Type:
    Content-Length:
    body
    */
    class http_response{
        public:
            std::string status;
            std::map<std::string, std::string> header;
            std::string body;
            std::string gen_http_response(std::string, std::map<std::string, std::string>, std::string);
    };

    class route{   
        public:
            std::string method;
            std::string route_path_s;
            std::string(*callback_func)(std::string);
            std::vector<std::string> routing_path_v;
            route(std::string, std::string(*)(std::string), std::string method);
    };

    class socket_process{
        private:
            int listen_port;
            sockpp::tcp_acceptor tcp_accept;
            std::vector<route> route_v;
        public:
            socket_process(int);
            void run_echo(sockpp::tcp_socket);
            void run_socket();
            void add_route(std::string, std::string(*)(std::string), std::string);
            std::string get_routing(std::string);
            std::string get_response(std::map<std::string, std::string>);
            std::map<std::string, std::string> analysis(std::string);
    };
};
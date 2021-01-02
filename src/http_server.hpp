#pragma once
#include <thread>
#include <vector>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"
#include <iostream>
#include <string>
#include <map>
#include <regex>
using namespace std;

//std::map<std::string, std::string> http_request_format;
//std::map<std::string, std::string> http_response_format;

class route{
    private:
        std::string method;
        std::map<std::string, void(*)(void*)> route_text_map_func;
    public:
        route(std::string, void(*)(void*), std::string method);
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
        void add_route(std::string, void(*)(void*), std::string method);
        std::map<std::string, std::string> get_response(std::map<std::string, std::string>);
        std::map<std::string, std::string> analysis(std::string);
};
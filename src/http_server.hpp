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

/** http response example*/
static std::string _200_response = "HTTP/1.1 200 OK\r\n";
static std::string _400_response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n";
static std::string _403_response = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n\r\n";
static std::string _404_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
static std::string _405_response = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n\r\n";

/** namespace http_server */
namespace http_server{
    /** from string analysis to route
    *  @param http_request routing string
    *  @return group of routing
    */
    std::vector<std::string> routing_analysis(std::string);
    /** class http_response*/
    class http_response{
        public:
            /** http response status*/
            std::string status;
            /** group of header */
            std::map<std::string, std::string> header;
            /** http body */
            std::string body;
            /** generate http response
            * @param status http response
            * @param header group of header
            * @param body http body
            * @return complete http response string
            */
            std::string gen_http_response(std::string, std::map<std::string, std::string>, std::string);
    };

    /** class route */
    class route{   
        public:
            /** http method */
            std::string method;
            /** routing */
            std::string route_path_s;
            /** callback function, whne receiving. */
            std::string(*callback_func)(std::string);
            /** routing vector */
            std::vector<std::string> routing_path_v;
            //! constructor.
            /**
            * @param route_path http routing path
            * @param callback callback function
            * @param method http method (ex: POST, GET)
            */
            route(std::string, std::string(*)(std::string), std::string method);
    };

    /** class socket process */
    class socket_process{
        private:
            /** listen port*/
            int listen_port;
            sockpp::tcp_acceptor tcp_accept;
            std::vector<route> route_v;
        public:
            //! constructor.
            /**
            * @param port listen port
            */
            socket_process(int);
            void run_echo(sockpp::tcp_socket);
            /** run socker forever */
            void run_socket();
            /** add route to listen's vector
            * @param route_body http routing path
            * @param callback callback function
            * @param method http method (ex: POST, GET)
            */
            void add_route(std::string, std::string(*)(std::string), std::string);
            /** from string, getting routing path
            * @param routing_path routing
            * @return string of route
            */
            std::string get_routing(std::string);
            /** add route to listen's vector
            * @param http_request_m map of http_request
            * @return complete string of http response
            */
            std::string get_response(std::map<std::string, std::string>);
            /** http request from string to map
            * @param http_request string of http_request
            * @return map of http_request
            */
            std::map<std::string, std::string> analysis(std::string);
    };
};
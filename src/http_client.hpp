#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>
#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"

namespace http_client{
    class http_request{
        public:
            std::string complete_http_request;
            //
            std::string route_path;
            std::string method;
            std::map<std::string, std::string> header;
            std::string content_length;
            std::string body;
            http_request(std::string, std::string, std::map<std::string, std::string>, \
                                        std::string);
            http_request(){};
    };

    class http_response{
        public:
            std::string http_version;
            std::string http_status;
            std::map<std::string, std::string> header_map_value;
            std::string body;
            http_response(){};
    };

    class http_client{
        private:
            int port;
            std::string host;
            sockpp::tcp_connector conn;
        public:
            http_client(std::string, int);
            //send request, and receive response
            void send_request(http_request, http_response*);
    };
};
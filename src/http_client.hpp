#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>
#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"

/** http_client contain method of http request. */
namespace http_client{
    //!  class http_request 
    /*!
    * contain http request structure.
    */
    class http_request{
        public:
            /**  complete http request frame. */
            std::string complete_http_request;
            /**  routing path */
            std::string route_path;
            /** http request method(ex: GET, POST, ...) */
            std::string method;
            /** group of header  */
            std::map<std::string, std::string> header;
            /** body length  */
            std::string content_length;
            /** body */
            std::string body;
            //! constructor. it contain generate complete_http_request
            /** 
            * @param route_path rouring path
            * @param method http request method
            * @param header_value http group of header
            * @param body body
            */
            http_request(std::string, std::string, std::map<std::string, std::string>, \
                                        std::string);
            http_request(){};
    };

    //! class http_response
    /*!
    * contain http response structure.
    */
    class http_response{
        public:
            /** http version.(ex: HTTP/1.1) */
            std::string http_version;
            /** http response status.(ex: 200) */
            std::string http_status;
            /** group of http header. */
            std::map<std::string, std::string> header_map_value;
            /** response's body. */
            std::string body;
            http_response(){};
    };

    //! class http_client
    /*!
    *   It contains tcp connect. 
    */
    class http_client{
        private:
            /** tcp listen port.*/
            int port;
            /** remote host */
            std::string host;
            sockpp::tcp_connector conn;
        public:
            //! constructor.
            /**
            * @param host remote host
            * @param port remote port
            */
            http_client(std::string, int);
            //! send request, and receive response.
            /** 
            * @param req http request structure
            * @param res when sending request to remote, getting response.
            */
            void send_request(http_request, http_response*);
    };
};
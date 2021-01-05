#include "http_client.hpp"

http_client::http_client::http_client(std::string host, int port){
    this->host = host;
    this->port = port;
    sockpp::socket_initializer sockInit;
}

void http_client::http_client::send_request(http_request req, http_response *res){
    // connect host
    this->conn.connect({this->host, (in_port_t)this->port});
    // send request
    this->conn.write(req.complete_http_request);
    // receive response
    char buf[25600] = {0};
    int read_size = this->conn.read(buf, sizeof(buf));
    // string temp region
    std::string response_tmp = "";
    std::string body = "";
    std::vector<std::string> response_v;
    // body section flag
    bool body_flag = false;
    for(int buf_idx = 0; buf_idx < read_size; buf_idx++){
        // hedaer part
        if(buf[buf_idx] == '\n' && buf[buf_idx-1] == '\r'){
            response_v.push_back(response_tmp);
            response_tmp.clear();
        }
        else{
            response_tmp.push_back(buf[buf_idx]);
        }
        // body
        if(buf[buf_idx-3] == '\r' && buf[buf_idx-2] == '\n' && buf[buf_idx-1] == '\r' && buf[buf_idx] == '\n'){
            body_flag = true;
        }
        if(body_flag)
            if(buf[buf_idx] != '\n' && buf[buf_idx] != '\r')
                body.push_back(buf[buf_idx]);
    }
    // put body
    res->body = body;
    // put http version
    std::smatch sm;
    std::regex_search(response_v[0], sm, std::regex("/[A-Za-z0-9.]*"));
    res->http_version = sm[0].str();
    std::regex_search(response_v[0], sm, std::regex("[0-9][0-9][0-9]*"));
    res->http_status = sm[0].str();
    // remove first element (http version and http status)
    response_v.erase(response_v.begin());
    // put header
    std::vector<std::string>::iterator response_begin_i = response_v.begin();
    std::vector<std::string>::iterator response_end_i = response_v.end();
    while(response_begin_i!=response_end_i){
        std::string header, value;
        std::string::iterator response_s_begin = response_begin_i->begin();
        std::string::iterator response_s_end = response_begin_i->end();
        bool header_value_flag = false;
        while(response_s_begin!=response_s_end){
            if(*response_s_begin == ':'){
                header_value_flag = true;
            }
            if(!header_value_flag)
                header.push_back(*response_s_begin);
            else
                value.push_back(*response_s_begin);
            response_s_begin++;
        }
        response_begin_i++;
        res->header_map_value[header] = value;
    }
    this->conn.close();
}

http_client::http_request::http_request(std::string route_path, std::string method, std::map<std::string, std::string> header_value, \
                                std::string body){
                            this->route_path = route_path;
                            this->method = method;
                            this->header = header;
                            this->content_length = std::to_string(body.size());
                            this->body = body;
                            // generate http
                            this->complete_http_request = method;
                            this->complete_http_request.append(" ");
                            this->complete_http_request.append(this->route_path);
                            this->complete_http_request.append(" ");
                            this->complete_http_request.append("HTTP/1.1\r\n");
                            std::map<std::string, std::string>::iterator header_value_begin = header_value.begin();
                            std::map<std::string, std::string>::iterator header_value_end = header_value.end();
                            while(header_value_begin!=header_value_end){
                                this->complete_http_request.append(header_value_begin->first);
                                this->complete_http_request.append(": ");
                                this->complete_http_request.append(header_value_begin->second);
                                this->complete_http_request.append("\r\n");
                                header_value_begin++;
                            }
                            this->complete_http_request.append("Content-Length");
                            this->complete_http_request.append(": ");
                            this->complete_http_request.append(this->content_length);
                            this->complete_http_request.append("\r\n");
                            this->complete_http_request.append(body);
                            this->complete_http_request.append("\r\n");
                            this->complete_http_request.append("\r\n");
}
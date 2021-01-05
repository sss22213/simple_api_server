#include "http_server.hpp"

std::string http_request_header[] = {"POST", "GET", "NULL"};

bool pair_string(std::string m_str, std::string sub_str){
    if(m_str.size() < 1 || sub_str.size() <  1)
        return false;
    for(int str_idx = 0; str_idx < (int)(m_str.size() - sub_str.size()); str_idx++){
        bool pair_result = true;
        for(int sub_str_idx = 0; sub_str_idx < (int)sub_str.size(); sub_str_idx++){
            if(m_str[str_idx + sub_str_idx] == sub_str[sub_str_idx])
                pair_result &= true;
            else
                pair_result &= false;
        }
        if(pair_result)
            return true;
    }
    return false;
}

http_server::socket_process::socket_process(int port){
    this->listen_port = port;
}

void http_server::socket_process::run_socket(){
    sockpp::tcp_acceptor acc(this->listen_port);
    while (true) {
        // Accept a new client connection
        sockpp::tcp_socket sock = acc.accept();
        if (!sock) {
            cerr << "Error accepting incoming connection: "
                << acc.last_error_str() << endl;
        }
        else {
            // Create a thread and transfer the new stream to it.
            thread thr(&socket_process::run_echo, this, std::move(sock));
            thr.detach();
        }
    }
}

void http_server::socket_process::run_echo(sockpp::tcp_socket sock){
    ssize_t n;
	char buf[25600];
    std::string response;
    if((n = sock.read(buf, sizeof(buf))) > 0){
        // analysis
        response = this->get_response(this->analysis(buf));
    }
    // bad request
    if(response.size() < 5){
        response = _400_response;
    }
    sock.write_n(response.c_str(), strlen(response.c_str()));
	cout << "Connection closed from " << sock.peer_address() << endl;
}

std::map<std::string, std::string> http_server::socket_process::analysis(std::string http_request){
    std::map<std::string, std::string> http_request_m;
    std::string body, tmp;
    bool header_body_flag = true;
    //
    std::vector<std::string> http_request_v;
    std::string::iterator str_begin = http_request.begin();
    std::string::iterator str_end = http_request.end();
    while(str_begin!=str_end){
        if(header_body_flag){
            // catch header
            if((*str_begin)=='\n' && *(str_begin-1)=='\r'){
                http_request_v.push_back(tmp);
                tmp.clear();
            }
            else{
                tmp.push_back(*str_begin);
            }
            if((*str_begin)=='\n' && *(str_begin-1)=='\r' && *(str_begin-2)=='\n' && *(str_begin-3)=='\r'){
                header_body_flag = false;
            }
        }
        else{
            body.push_back(*str_begin);
        }
        str_begin++;
    }
    http_request_m["method"] = http_request_v[0]; // http method
    http_request_m["body"] = body; // body 
    http_request_m["route"] = get_routing(http_request_v[0]); // route
    // header
    std::vector<std::string>::iterator http_request_begin_i = http_request_v.begin();
    std::vector<std::string>::iterator http_request_end_i = http_request_v.end();
    while(http_request_begin_i != http_request_end_i){
        int header_tail_loc = (*http_request_begin_i).find(":");
        if(header_tail_loc > 0)
            http_request_m[(*http_request_begin_i).substr(0, header_tail_loc)] = \
                        (*http_request_begin_i).substr(header_tail_loc + 2, (*http_request_begin_i).size()-1);
        http_request_begin_i++;
    }
    return http_request_m;
}

http_server::route::route(std::string route_path, std::string(*callback)(std::string), std::string method){
    // this->route_text_map_func[route_path] = callback;
    this->callback_func = callback;
    this->method = method;
    this->routing_path_v = routing_analysis(route_path);
    this->route_path_s = route_path;
}

void http_server::socket_process::add_route(std::string route_body, std::string(*callback)(std::string), std::string method){
    route_v.push_back(route(route_body, callback, method));
}

std::string http_server::socket_process::get_response(std::map<std::string, std::string> http_request_m){
    std::string method;
    std::vector<std::string> routing;
    // get method
    std::string *ptr_method = http_request_header;
    while((*ptr_method) != "NULL"){
        if(pair_string(http_request_m["method"], *ptr_method)){
            method = *ptr_method;
            break;
        }
        ptr_method++;
    }
    // find route
    std::vector<route>::iterator route_v_begin = route_v.begin();
    std::vector<route>::iterator route_v_end = route_v.end();
    while(route_v_begin!=route_v_end){
        if(route_v_begin->route_path_s.compare(http_request_m["route"]) == 0){
            if(method == route_v_begin->method){
                return route_v_begin->callback_func(http_request_m["body"]);
            }
            else
                // method not found
                return _405_response;
            break;
        }
        route_v_begin++;
    }
    return _404_response;
}

std::vector<std::string> http_server::routing_analysis(std::string route_path){
    std::vector<std::string> routing_path_v;
    // get routing
    smatch sm;
    while(regex_search(route_path, sm, regex("/[A-Za-z0-9]*"))){
        // ripple
        if(sm.str().compare("/1"))
            routing_path_v.push_back(sm.str());
        route_path = sm.suffix();
    }
    return routing_path_v;
}

std::string http_server::socket_process::get_routing(std::string routing_path){
    int block = 0;
    std::string route_str;
    while(!routing_path.empty()){
        char route_c = routing_path.front();
        routing_path.erase(routing_path.begin());
        if(block == 1){
            if(route_c!=' ')
                route_str.push_back(route_c);
        }
        else if(block == 2)
            break;
        if(block == 0 && route_c ==' ')
            block = 1;
        else if(block == 1 && route_c ==' ')
            block = 2;
    }
    return route_str;
}

std::string http_server::http_response::gen_http_response(std::string status, std::map<std::string, std::string> header, \
                                            std::string body){
    //
    this->status = status;
    this->header = header;
    this->body = body;
    //
    std::string http_response_frame = "HTTP/1.1 ";
    http_response_frame.append(status);
    http_response_frame.append("\r\n");
    std::map<std::string, std::string>::iterator header_begin = header.begin();
    std::map<std::string, std::string>::iterator header_end = header.end();
    while(header_begin!=header_end){
        http_response_frame.append(header_begin->first);
        http_response_frame.append(": ");
        http_response_frame.append(header_begin->second);
        http_response_frame.append("\r\n");
        header_begin++;
    }
    http_response_frame.append("Content-Length: ");
    http_response_frame.append(std::to_string(body.size()));
    http_response_frame.append("\r\n\r\n");
    http_response_frame.append(body);
    http_response_frame.append("\r\n");
    return http_response_frame;
};
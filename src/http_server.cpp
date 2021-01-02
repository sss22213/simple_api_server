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

socket_process::socket_process(int port){
    this->listen_port = port;
}

void socket_process::run_socket(){
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

void socket_process::run_echo(sockpp::tcp_socket sock){
    ssize_t n;
	char buf[25600];
    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    if((n = sock.read(buf, sizeof(buf))) > 0){
        this->analysis(buf);
    }
    sock.write_n(response, strlen(response));
	cout << "Connection closed from " << sock.peer_address() << endl;
}

route::route(){

}

std::map<std::string, std::string> socket_process::analysis(std::string http_request){
    //
    std::map<std::string, std::string> http_request_m; 
    // http request
    std::vector<std::string> http_request_v;
    std::string::iterator str_begin = http_request.begin();
    std::string::iterator str_end = http_request.end();
    std::string tmp;
    while(str_begin!=str_end){
        if((*str_begin)=='\n' && *(str_begin-1)=='\r'){
            http_request_v.push_back(tmp);
            tmp.clear();
        }
        else{
            tmp.push_back(*str_begin);
        }
        str_begin++;
    }
    // analysis
    // method
    std::string *ptr_method = http_request_header;
    while((*ptr_method) != "NULL"){
        if(pair_string(http_request_v[0], *ptr_method)){
            http_request_m["method"] = *ptr_method;
            break;
        }
        ptr_method++;
    }
    // route
    smatch sm;
    regex_search(http_request_v[0], sm, regex("/[A-Za-z0-9]*"));
    http_request_m["route"] = sm[0];
    // get header
    std::vector<std::string>::iterator http_request_begin_i = http_request_v.begin();
    std::vector<std::string>::iterator http_request_end_i = http_request_v.end();
    while(http_request_begin_i != http_request_end_i){
        int header_tail_loc = (*http_request_begin_i).find(":");
        if(header_tail_loc > 0)
            http_request_m[(*http_request_begin_i).substr(0, header_tail_loc)] = \
                        (*http_request_begin_i).substr(header_tail_loc + 2, (*http_request_begin_i).size()-1);
        std::cout << *http_request_begin_i << std::endl;
        http_request_begin_i++;
    }
}
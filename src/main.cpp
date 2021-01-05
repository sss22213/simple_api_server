#include "http_server.hpp"
#include "http_client.hpp"

static std::string get_ip(void){
    http_client::http_client http_client1("api.ipify.org", 80);
    // http request -> http response
    http_client::http_response http_response1;
    // put http request header
    std::map<std::string, std::string> header;
    header["HOST"] = "api.ipify.org";
    header["Connection"] = "keep-alive";
    http_client::http_request http_request("/", "GET", header, "");
    // send http request
    http_client1.send_request(http_request, &http_response1);
    return http_response1.body;
}

std::string home(std::string body){
    // http response
    http_server::http_response http_response1;
    std::map<std::string, std::string> response;
    response["Content-Type"] = "application/json";
    return http_response1.gen_http_response("200 OK", response, "{\"IP\": \""+ get_ip() +"\", \"result\": \"200\"}");
}


int main(){
    get_ip();
    http_server::socket_process socket_process1(7800);
    socket_process1.add_route("/api/v1/get_global_ip/4AAQSkZJRgABAQAAAQABAAD2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw", home, "POST");
    socket_process1.run_socket();
    return 0;
}

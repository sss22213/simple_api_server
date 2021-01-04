#include "http_server.hpp"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

std::string serverip_get(){
    try{
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        std::ostringstream response;
        request.setOpt(new curlpp::options::Url("https://api.ipify.org")); 
        request.setOpt(new curlpp::options::Verbose(true)); 
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform();
        return response.str();
    }
    catch(...){
        std::cout << "Server is down" << std::endl;
        return "";
    }

}

std::string home(std::string body){
    // http response
    http_response http_response1;
    std::map<std::string, std::string> response;
    response["Content-Type"] = "application/json";
    return http_response1.gen_http_response("200 OK", response, "{\"IP\": \""+ serverip_get() +"\", \"result\": \"200\"}");
}

int main(){
    socket_process socket_process1(7800);
    socket_process1.add_route("/api/v1/get_global_ip/4AAQSkZJRgABAQAAAQABAAD2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw", home, "POST");
    socket_process1.run_socket();
    return 0;
}

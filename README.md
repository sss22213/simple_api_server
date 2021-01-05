simple_api_server
===

This repository contains simple http server and http client

## Feature:
- using tcp.
- http server only support method of post and get
- http client only support http

## Depend:

  - Install sockpp
```bash
git clone https://github.com/fpagliughi/sockpp
cd sockpp
mkdir build; cd build
cmake ..
make
sudo make install
sudo ldconfig
```

## Install:
```bash
mkdir build
cd build
cmake ..
make
```
## Example:

```C++
#include "http_server.hpp"
#include "http_client.hpp"

static std::string get_ip(void){
    
    http_client::http_client http_client1("api.ipify.org", 80); // declare http client http_client::http_client(std::string host, iny port)
    
    http_client::http_response http_response1; // declare http_response http_client::http_response::http_response()
    // create http header and value
    std::map<std::string, std::string> header;
    header["HOST"] = "api.ipify.org";
    header["Connection"] = "keep-alive";
    http_client::http_request http_request("/", "GET", header, ""); declare http_request http_client::http_request(std::string route, std::string method, std::map<std::string, std::string> header_value, std::string body)
    http_client1.send_request(http_request, &http_response1); // send http request http_client::http_request::send_request(http_client::http_request, http_client::http_response)
    return http_response1.body;
}

std::string home(std::string body){
    http_server::http_response http_response1; // declare http_response http_server::http_response::http_response()
    std::map<std::string, std::string> response;
    response["Content-Type"] = "application/json";
    // gen_http_response http_server::http_response::gen_http_response(std::string http_status, std::map<std::string, std::string> response_header_value, std::string body)
    return http_response1.gen_http_response("200 OK", response, "{\"IP\": \""+ get_ip() +"\", \"result\": \"200\"}");
}


int main(){
    // declare http_server http_server::socket_process::socket_process(int listen_port)
    http_server::socket_process socket_process1(7800);
    // add_route http_server::socket_process::add_route(std::string route, std::string(*callback)(std::string), std::string method)
    socket_process1.add_route("/", home, "POST");
    // run socket
    socket_process1.run_socket();
    return 0;
}

```


# Refenerce
* sockpp: https://github.com/fpagliughi/sockpp.git


#include "http_server.hpp"
int main(){
    socket_process socket_process1(7800);
    socket_process1.run_socket();
    return 0;
}

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <list>
#include <map>
#include <string>
#include <iterator>
#include "HttpRequest.hpp"
#include "Response.hpp"
# include <iostream>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <poll.h>
# include <algorithm>
# include <sys/stat.h>
# include <fcntl.h>
#define PORT 8089

void    splitting_it(char *test)
{
    
    char *ok = strtok(test, ": ");
    char *ptr = strtok(NULL, " ");

    std::cout << ok << std::endl;
    std::cout << ptr << std::endl;
}

int main(int argc, char const *argv[])
{
    char buffer[30000] = {0};
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Only this line has been changed. Everything is same.
   // char *hello = strdup("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    // if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    // {
    //     perror("In bind");
    //     exit(EXIT_FAILURE);
    // }
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    std::string data = "";
    std::string res;
    size_t body_size = 0;
    while(1)
    {
        //printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        HttpRequest http;
        http.handle_http_request(new_socket);
        if (http.Get_Http_Method() == "POST" && http.get_value("Transfer-Encoding") == "chunked")
        {
            std::cout << "ok?" << std::endl;
            http.handle_chunked_body();
        }
        Response ok;
        ok.set_mybuffer(http.Get_Request_Target());
        if (http.Get_Http_Method() == "GET")
            body_size = ok.handle_Get_response();
        else if (http.Get_Http_Method() == "DELETE")
            ok.handle_delete_response(http.get_value("Connection"));
        else if (http.Get_Http_Method() == "POST")
            ok.handle_post_response(http.get_value("Connection"));
        std::cout << ok.get_hello() << std::endl;
        
        write(new_socket , ok.get_hello() , ok.get_total_size());
        close(new_socket);
        break;
    }
    return 0;
}
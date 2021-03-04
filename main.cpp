#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

void printHelp();

int main (int argc, char** argv) {
    if (argc != 2){
        printHelp();
        return 2;
    }

    std::cout << "Given IP/DNS address = " << argv[1] << std::endl;

    char ipstr[INET6_ADDRSTRLEN];

    struct addrinfo hints, *res, *list;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; //AF_INET or AF_INET6 if needed
    hints.ai_socktype = SOCK_STREAM; //tcp

    int rc;
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &res)) !=0){
        std::cout << "getaddrinfo returned error: " << gai_strerror(rc) << std::endl;
        return 1;
    }

    for (list = res; list!=NULL; list = list->ai_next){
        void* addr;
        const char* ipver;

        if (list->ai_family == AF_INET){
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)list->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }else{
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)list->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(list->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << "Address = " << ipver << " " << ipstr << std::endl;
    }
    freeaddrinfo(res);
    return 0;
}

void printHelp(){
    std::cout << "Program prints IP addresses of given host. DNS/ip address accepted as arg" << std::endl;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/time.h> 



int client_socket[30], max_clients = 30, master_socket;

void signal_handler (int signum) {
    if (signum != SIGINT) {
        return;
    }
    for (int i = 0; i < max_clients; ++i) {
        int sd = client_socket[i];
        close(sd);
        client_socket[i] = 0;
    }
    close(master_socket);        
    exit(0);   
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n\t./a.out port\n");
        exit(1);
    }
    int PORT = atoi(argv[1]);
    int opt = 1;
    int addrlen, new_socket, activity, valread, sd;  
    int max_sd;
    struct sockaddr_in address;
    char buffer[1025];

    fd_set readfds;
 
    for (int i = 0; i < max_clients; ++i) {
        client_socket[i] = 0;
    }

    master_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (master_socket < 0) {
        perror("socket");
        exit(1);
    }
 
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {  
        perror("setsockopt");  
        exit(1);  
    }
    
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );
 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }
 
    if (listen(master_socket, 3) < 0) {
        perror("listen");
        return 1;
    }
    
    addrlen = sizeof(address);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, 0);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;
        
        for (int i = 0; i < max_clients; i++) {  
            sd = client_socket[i];  
            if(sd > 0) {
                FD_SET( sd , &readfds);  
            }   
            if(sd > max_sd) {
                max_sd = sd; 
            } 
        } 
        
        if (activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL) < 0) {
            perror("select");
        }
        
        if (FD_ISSET(master_socket, &readfds)) {  
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {  
                perror("accept");  
                exit(1);  
            }
            for (int i = 0; i < max_clients; i++) {
                if( client_socket[i] == 0 ) {  
                    client_socket[i] = new_socket;
                    break;  
                }  
            }  
        }
        for (int i = 0; i < max_clients; i++) {  
            sd = client_socket[i];  
            if (FD_ISSET( sd , &readfds)) {  
                if ((valread = read( sd , buffer, 1024)) < 0) {  
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    close(sd);  
                    client_socket[i] = 0;  
                }  
                else {  
                    buffer[valread] = '\0';
                    if (!strncmp(buffer, "quit", 4)) {
                        close(sd);
                        client_socket[i] = 0;
                    }  
                    printf("Client%d:%s", i+1, buffer);
                }  
            }  
        }
    }   

    close(master_socket);
    return 0;
}

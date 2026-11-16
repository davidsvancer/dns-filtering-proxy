// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns.c
 * @author David Švancer
 *
 * @brief Implementation of DNS resolver.
 */

#include "../include/dns.h"

int main(int argc, char **argv) {

    // create param structure for args parse
    Parameters * param = malloc(sizeof(Parameters));
    if (param == NULL) {
        
        perror("Allocating memory failed");
        return EXIT_FAILURE;
    }

    int param_parse_result = Parse_Arguments(argc, argv, param);
    if (param_parse_result == FUNCTION_ERROR) {      // wrong input args

        Clean_Memory(param, NULL, NULL, NULL);
        return EXIT_FAILURE; 
    }               
    if (param_parse_result == FUNCTION_PRINT_HELP) {        // help
        
        Clean_Memory(param, NULL, NULL, NULL);
        return EXIT_SUCCESS;
    }
                            
    verbose_enabled = param->verbose;                   // enables logs

    Node ** list = malloc(sizeof(Node));
    if (list == NULL) {

        perror("Allocating memory failed");
        Clean_Memory(param, NULL, NULL, NULL);
        return EXIT_FAILURE;
    }
    *list = NULL;
    int file_read_result = Read_File(param->file, list);               // read file and save its context to linked list
    if (file_read_result == FUNCTION_ERROR) {

        Clean_Memory(param, list, NULL, NULL);
        return EXIT_FAILURE;
    }

    // inicialization of socket for comunication with client
    // using it as dual stack socket so it can be used with both IPv4/IPv6
    // set up with socketoption(), IPv4 are mapped
    int client_socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    int opt = 0;
    setsockopt(client_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
    if (client_socket_fd < 0) {

        perror("Creating socket for client communication failed");
        Clean_Memory(param, list, NULL, NULL);
        return EXIT_FAILURE;
    }

    Socket *client_socket = malloc(sizeof(Socket));
    if (client_socket == NULL) {
        
        perror("Allocating memory for client socket failed");
        Clean_Memory(param, list, NULL, NULL);
        close(client_socket_fd);
        return EXIT_FAILURE;
    }

    // Prepare socket values
    struct sockaddr_in6 * client_address = (struct sockaddr_in6 *)&client_socket->address;
    memset(client_address, 0, sizeof(*client_address));
    client_address->sin6_family = AF_INET6;
    client_address->sin6_port = htons(param->port);
    client_address->sin6_addr = in6addr_any;
    client_socket->socket_fd = client_socket_fd;

    // bind to given port from args, receiving any address IPv4/IPv6
    int bind_result = bind(client_socket->socket_fd, (struct sockaddr *)&client_socket->address, sizeof(client_socket->address));
    if (bind_result < 0) {
       
        perror("Binding socket failed");
        Clean_Memory(param, list, client_socket, NULL);
        close(client_socket_fd); 
        return EXIT_FAILURE;        
    }

    // Socket for comunication with given DNS resolver
    Socket *server_socket = malloc(sizeof(Socket));
    if (server_socket == NULL) {

        perror("Allocating memory for server socket failed");
        Clean_Memory(param, list, client_socket, NULL);
        close(client_socket_fd); 
        return EXIT_FAILURE;
    }

    // structures needed in getting address info
    // using getaddrinfo() we get ip address and type
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        // IPv4 + IPv6
    hints.ai_socktype = SOCK_DGRAM;   // UDP socket

    int err = getaddrinfo(param->server, "53", &hints, &result);            // 53 is default DNS port
    if (err != 0) {

        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        Clean_Memory(param, list, client_socket, server_socket);
        close(client_socket_fd); 
        return EXIT_FAILURE;
    }

    int server_socket_fd = socket(result->ai_family, SOCK_DGRAM, 0);
    struct timeval tv;                      // for seting up the timeout for recfrom()
    tv.tv_sec = 2;                          // timeout 2s
    tv.tv_usec = 0;
    server_socket->socket_fd = server_socket_fd;
    setsockopt(server_socket->socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (server_socket_fd < 0) {

        perror("Creating socket for server communication failed");
        Clean_Memory(param, list, client_socket, server_socket);
        close(client_socket_fd); close(server_socket_fd);
        return EXIT_FAILURE;
    }

    memcpy(&server_socket->address, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    Log_Info("INFO: Everything inicializated successfully, now listening on port: %d", param->port);      

    // Infinite loop for getting client sockets
    while(1) {

        unsigned char buffer[512];
        struct sockaddr_storage client_address;
        socklen_t client_address_size = sizeof(client_address);
        ssize_t received_bytes = recvfrom(client_socket->socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_size);
        if (received_bytes < 0) {
                                                                // loop continues 
            Log_Info("ERROR: Error receiving data from client");
            continue; 
        }

        Thread_Arguments * thread_args = malloc(sizeof(Thread_Arguments));
        if (thread_args == NULL) {
    
            Log_Info("ERROR: Allocating memory for thread arguments failed");
            continue;
        }
        // save all needed datas to thread arguments, so we can pass it to the new thread
        thread_args->received_bytes = received_bytes;
        memcpy(thread_args->buffer, buffer, received_bytes);
        memcpy(&thread_args->client_addr, &client_address, client_address_size);
        thread_args->client_address_size = client_address_size;
        thread_args->server_socket = server_socket;
        thread_args->client_socket = client_socket;
        thread_args->list = list;

        // for each client request is new thread created
        pthread_t thread;
        if (pthread_create(&thread, NULL, Handle_Request, thread_args) != 0) {

            Log_Info("ERROR: Creating new thread failed");
            free(thread_args);
            continue;
        }

        pthread_detach(thread);
    }

    // clean 
    close(client_socket_fd);
    close(server_socket_fd);
    Clean_Memory(param, list, client_socket, server_socket);
    return EXIT_SUCCESS;
}

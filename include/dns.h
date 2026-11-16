// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns.h
 * @author David Švancer
 *
 * @brief Header file for DNS resolver implementation.
 */
#ifndef DNS_H
#define DNS_H

// fedora needs this
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif
//

#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include "log.h"
#include "arg_parse.h"
#include "linked_list.h"
#include "dynamic_array.h"
#include "utils.h"
#include "thread.h"
#include "dns_packet_create.h"
#include "dns_packet_parse.h"

typedef struct Header_Flags {

    uint8_t qr;       // 0 = query, 1 = response
    uint8_t opcode;   // 4 bite
    uint8_t aa;       // authoritative answer
    uint8_t tc;       // truncated
    uint8_t rd;       // recursion desired
    uint8_t ra;       // recursion available
    uint8_t z;        // reserved, 3 bite
    uint8_t rcode;    // response code, 4 bite
} Header_Flags;

typedef struct Header {

    uint16_t id;             // ID 
    Header_Flags flags;      
    uint16_t qd_count;       // number of questions
    uint16_t an_count;       // number of responses
    uint16_t ns_count;       // number of authorites
    uint16_t ar_count;       // number of ar
} Header;

typedef struct Question {

    char * qname;       
    unsigned short qtype;    // type
    unsigned short qclass;  
} Question;

typedef struct Qname_struc {

    char * name;
    int offset;
} Qname_struc;

typedef struct Socket{

    int socket_fd;                    // file descriptor 
    struct sockaddr_storage address;  // both iPv4/iPv6
} Socket;

typedef struct Thread_Arguments {
    
    unsigned char buffer[512];
    struct sockaddr_storage client_addr;
    socklen_t client_address_size;
    ssize_t received_bytes;
    Socket * server_socket;
    Socket * client_socket;
    Node ** list;
} Thread_Arguments;

/**
 * @brief Main DNS function.
 * @param[in] argc Argument count.
 * @param[in] argv Double pointer, which contains input arguments.
 * @return returns EXIT_FAILURE if error occurs, otherwise EXIT_SUCCESS
 */
int main(int argc, char **argv);

#endif

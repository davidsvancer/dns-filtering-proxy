// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns_packet_create.c
 * @author David Švancer
 *
 * @brief Implementation of thread handling.
 */

#include "../include/thread.h"

void * Handle_Request(void *args) {

    Thread_Arguments *thread_args = (Thread_Arguments *)args;
    unsigned char buffer[512];
    unsigned char reminder[512];
    int reminder_size = 0;
    int packet_size = 0;
    int parse_result;

    Header * header = malloc(sizeof(Header));
    if (header == NULL) {

        Log_Info("ERROR: Allocating memory failed");
        free(thread_args);
        pthread_exit(NULL);
    }
    Question * question = malloc(sizeof(Question));
    if (question == NULL) {

        Log_Info("ERROR: Allocating memory failed");
        Clean_Memory_And_Exit_Thread(thread_args, header, NULL);
    }

    // Parse packet and save its data to structures for later usage
    parse_result = DNS_Parse_Packet(thread_args->buffer, thread_args->received_bytes, header, question, reminder, &reminder_size);
    if (parse_result == FUNCTION_ERROR) {

        Clean_Memory_And_Exit_Thread(thread_args, header, question);
    }

    uint16_t client_id = header->id; // save ID before changing

    // Validate domain and type
    if (!Is_Packet_Type_Valid(question) || !Is_Valid_Domain(*thread_args->list, question->qname)) {

        Log_Info("INFO: Invalid domain or type, sending back to the client request refused");
        header->flags.qr = 1;
        header->flags.rcode = 5; // REFUSED
        packet_size = DNS_Create_Packet(buffer, header, question, reminder, reminder_size);
        if (packet_size == FUNCTION_ERROR) {

           Clean_Memory_And_Exit_Thread(thread_args, header, question);
        }
        ssize_t sent_bytes = sendto(thread_args->client_socket->socket_fd, buffer, packet_size, 0, (struct sockaddr *)&thread_args->client_addr, thread_args->client_address_size);
        if (sent_bytes < 0) {
            
            Log_Info("ERROR: Error while sending packet");
        }

        Clean_Memory_And_Exit_Thread(thread_args, header, question);
    }

    Log_Info("INFO: Parse was successful, forwarding message to DNS server");

    header->id++;   // increment header id

    // Build Packet
    packet_size = DNS_Create_Packet(buffer, header, question, reminder, reminder_size);
    if (packet_size == FUNCTION_ERROR) {

        Clean_Memory_And_Exit_Thread(thread_args, header, question);
    }

    int max_retries = 3;
    ssize_t received_bytes = -1;

     // dns address saved from responce
    struct sockaddr_storage dns_resolver_address;
    socklen_t dns_resolver_address_len = sizeof(dns_resolver_address);

    for (int attempt = 0; attempt <= max_retries; attempt++) {

        // Send request to the DNS server
        ssize_t sent_bytes = sendto(thread_args->server_socket->socket_fd, buffer, packet_size, 0, (struct sockaddr *)&thread_args->server_socket->address,
                                    sizeof(thread_args->server_socket->address));
        if (sent_bytes < 0) {

            Log_Info("ERROR: Error while sending socket to DNS server");
            continue;
        }

        // Receive response from DNS server
        received_bytes = recvfrom(thread_args->server_socket->socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&dns_resolver_address,
                                    &dns_resolver_address_len);
        if (received_bytes >= 0) {
            
            break;  // success
        }

        if (errno == EAGAIN || errno == EWOULDBLOCK) {

            Log_Info("INFO: Timout, waiting for responce");
        } else {
            Log_Info("ERROR: Error while waiting for response from DNS server");
            break;
        }
    }

    // Server did not responsed
    if (received_bytes < 0) {

        Log_Info("INFO: Server did not respond, sending request refused to the client");
        header->flags.qr = 1;
        header->flags.rcode = 5; // REFUSED
        packet_size = DNS_Create_Packet(buffer, header, question, reminder, reminder_size);
        if (packet_size == FUNCTION_ERROR) {

            Clean_Memory_And_Exit_Thread(thread_args, header, question);
        }
        ssize_t sent_bytes = sendto(thread_args->client_socket->socket_fd, buffer, packet_size, 0, (struct sockaddr *)&thread_args->client_addr, 
                                        thread_args->client_address_size);
        if (sent_bytes < 0) {
            
            Log_Info("ERROR: Error while sending packet");
        }
        Clean_Memory_And_Exit_Thread(thread_args, header, question);
    }

    Log_Info("INFO: Successfuly received response from DNS server");

    // Parse only header, rest save to reminder and restore client ID 
    parse_result = DNS_Parse_Header_Only(buffer, packet_size, header, reminder, &reminder_size);
    if (parse_result == FUNCTION_ERROR) {

        Clean_Memory_And_Exit_Thread(thread_args, header, question);
    }
    header->id = client_id;

    // create packet 
    packet_size = DNS_Create_Packet_Header_Only(buffer, header, reminder,  reminder_size);
    if (packet_size == FUNCTION_ERROR) {

        Clean_Memory_And_Exit_Thread(thread_args, header, question);
    }

    // send response from DNS server back to the client
    ssize_t reply_sent = sendto(thread_args->client_socket->socket_fd, buffer, received_bytes, 0, (struct sockaddr *)&thread_args->client_addr,
                                    thread_args->client_address_size);

    if (reply_sent < 0) {
        
        Log_Info("ERROR: Sending socket back to client failed");
    } else {

        Log_Info("INFO: Sent response successfully back to the client");
    } 

    Clean_Memory_And_Exit_Thread(thread_args, header, question);
    return NULL; // Silence the warning 
}
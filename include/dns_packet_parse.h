// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns_packet_parse.h
 * @author David Švancer
 *
 * @brief Header file for packet parsing implementation.
 */
#ifndef DNS_PACKET_PARSE_H
#define DNS_PACKET_PARSE_H

#include "dns.h"

typedef struct Header Header;
typedef struct Question Question;

/**
 * @brief Function to parse the DNS header from a packet.
 * @param[in] packet The packet array to read from.
 * @param[in] packet_size Size of the packet in bytes.
 * @param[in] header Pointer to the Header structure to store parsed data.
 * @return Returns FUNCTION_SUCCESS if no error occured, otherwise FUNCTION_ERROR.
 */
int DNS_Parse_Header(unsigned char packet[], ssize_t packet_size, Header * header);


/**
 * @brief Function to parse a DNS question from a packet.
 * @param[in] packet The DNS packet array to read from.
 * @param[in] packet_size Size of the packet in bytes.
 * @param[in] question Pointer to the Question structure to store parsed data.
 * @return Returns the number of bytes read from the packet, or FUNCTION_ERROR on failure.
 */
int DNS_Parse_Question(unsigned char packet[], ssize_t packet_size,  Question * question);

/**
 * @brief Function to parse a full DNS packet including header and question, saving remainder.
 * @param[in] packet The packet array to read from.
 * @param[in] packet_size Size of the packet in bytes.
 * @param[in] header Pointer to the Header structure to store parsed data.
 * @param[in] question Pointer to the Question structure to store parsed data.
 * @param[in] reminder Buffer to store remaining packet data after parsing.
 * @param[in,out] reminder_size Pointer to store the size of the remaining data.
 * @return Returns FUNCTION_SUCCESS if no error occured, otherwise FUNCTION_ERROR.
 */
int DNS_Parse_Packet(unsigned char *packet, ssize_t packet_size, Header * header, Question * question, unsigned char * reminder, int * reminder_size);

/**
 * @brief Function to check if the DNS question type is valid (type: A | AAAA).
 * @param[in] question Pointer to the Question.
 * @return Returns true if the question type is valid, otherwise false.
 */
bool Is_Packet_Type_Valid(Question * question);

/**
 * @brief Function to parse only the DNS header and save the remainder of the packet.
 * @param[in] packet The DNS packet array to read from.
 * @param[in] packet_size Size of the packet in bytes.
 * @param[in] header Pointer to the Header structure to store parsed data.
 * @param[in] reminder Buffer to store remaining packet data.
 * @param[in,out] reminder_size Pointer to store the size of the remaining data.
 * @return Returns FUNCTION_SUCCESS if no error occured, otherwise FUNCTION_ERROR.
 */
int DNS_Parse_Header_Only(unsigned char * packet, ssize_t packet_size, Header * header, unsigned char * reminder, int * reminder_size);

/**
 * @brief Function to copy data from given index into the reminder buffer.
 * @param[in] reminder Buffer where the remaining packet data will be stored.
 * @param[in,out] reminder_size Pointer to store the size of the remaining data.
 * @param[in] packet The DNS packet array to read from.
 * @param[in] packet_size Size of the packet in bytes.
 * @param[in] offset From where we should copy data.
 */
void DNS_Parse_From(unsigned char * reminder, int * reminder_size, unsigned char * packet, int packet_size, int offset);

#endif

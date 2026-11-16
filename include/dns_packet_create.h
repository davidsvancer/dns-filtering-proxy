// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns_packet_create.h
 * @author David Švancer
 *
 * @brief Header file for packet creation implementation.
 */
#ifndef DNS_PACKET_CREATE_H
#define DNS_PACKET_CREATE_H

#include "dns.h"

typedef struct Header Header;
typedef struct Question Question;
typedef struct Qname_struc Qname_struc;

/**
 * @brief Function to build the QNAME for a DNS packet.
 * @param[in] hostname The domain name to convert into QNAME.
 * @param[in] packet The DNS packet array where the QNAME will be written.
 * @param[in] packet_offset The offset in the packet to start writing.
 * @param[in] qname_array Array of Qname_struc to store QNAMES info.
 * @param[in,out] qname_count Pointer to store the number of QNAME labels.
 * @return Returns the number of bytes written to the packet, or FUNCTION_ERROR if anything fails.
 */
int Build_QNAME(const char *hostname, unsigned char *packet, int packet_offset, Qname_struc *qname_array, int *qname_count);

/**
 * @brief Function to add the reminder to the packet.
 * @param[in] packet The DNS packet array to copy to.
 * @param[in] index The starting index in the packet to copy.
 * @param[in] reminder Reminder data buffer.
 * @param[in] reminder_size Size of the reminder buffer.
 * @return Returns the buffer size, otherwise FUNCTION_ERROR.
 */
int DNS_Add_Reminder(unsigned char * packet, int index, unsigned char * reminder, int reminder_size);


/**
 * @brief Function to add a DNS header to a packet.
 * @param[in] packet The DNS packet array where the header will be written.
 * @param[in] header Pointer to the header.
 */
void DNS_Add_Header(unsigned char * packet, Header * header);


/**
 * @brief Function to add a DNS question to a packet.
 * @param[in] packet The DNS packet array where the question will be written.
 * @param[in] question Pointer to the question.
 * @param[in] names Array of Qname_struc used for compression.
 * @param[in,out] names_count QName count.
 * @return Returns the number of bytes written to the packet, otherwise FUNCTION_ERROR.
 */
int DNS_Add_Question(unsigned char * packet, Question * question, Qname_struc * names, int * names_count);

/**
 * @brief Function to create a DNS packet with header and reminder.
 * @param[in] packet The DNS packet array where the packet will be written.
 * @param[in] header Pointer to the header.
 * @param[in] reminder Reminder data.
 * @param[in] reminder_size Size of the reminder buffer.
 * @return Returns the total size of the packet, otherwise FUNCTION_ERROR.
 */
int DNS_Create_Packet_Header_Only(unsigned char *packet, Header * header, unsigned char *reminder, int reminder_size);


/**
 * @brief Function to create a full DNS packet including header, question, and reminder.
 * @param[in] packet The DNS packet array where the packet will be written.
 * @param[in] header Pointer to the header.
 * @param[in] question Pointer to the question.
 * @param[in] reminder Reminder data.
 * @param[in] reminder_size Size of the reminder buffer.
 * @return Returns the total size of the packet, otherwise FUNCTION_ERROR.
 */
int DNS_Create_Packet(unsigned char *packet, Header *header, Question *question, unsigned char * reminder, int reminder_size);

#endif

// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns_packet_create.c
 * @author David Švancer
 *
 * @brief Implementation of packet creating.
 */

#include "../include/dns_packet_create.h"

int Build_QNAME(const char *hostname, unsigned char *packet, int packet_offset, Qname_struc * qname_array, int * qname_count) {

    int offset = 0;  
    int packet_start = packet_offset;
    int result;

    // remember where  QNAME starts in the packet
    int qname_start_offset = packet_offset;

    Dynamic_String * name = malloc(sizeof(Dynamic_String));
    if(name == NULL) {

        Log_Info("ERROR: Allocating memory failed");
        return FUNCTION_ERROR;
    }

    result = Array_Init(name, 30);
    if (result == FUNCTION_ERROR) {

        free(name);
        return FUNCTION_ERROR;
    }

    while (hostname[offset] != '\0') {

        int count = 0;
        // we count how many chars before '.'
        while (hostname[offset + count] != '.' && hostname[offset + count] != '\0') {

            count++;
        }
        
        packet[packet_offset++] = count;    // length byte
        for (int i = 0; i < count; i++) {               // copy label

            packet[packet_offset++] = hostname[offset + i];
            result = Array_AddChar(name, hostname[offset + i]);
            if (result == FUNCTION_ERROR) {

                Array_Free(name);
                free(name);
                return FUNCTION_ERROR;
            }
        }

        // add '.' to the name if there is more
        if (hostname[offset + count] == '.') {

            result = Array_AddChar(name, '.');
            if (result == FUNCTION_ERROR) {

                Array_Free(name);
                free(name);
                return FUNCTION_ERROR;
            }
            offset += count + 1;
        } else {

            offset += count;
        }
    }

    // end of QNAME
    packet[packet_offset++] = 0x00;
    result = Array_Terminate(name);
    if (result == FUNCTION_ERROR) {

        Array_Free(name);
        free(name);
        return FUNCTION_ERROR;
    }

    // save name and offset
    qname_array[*qname_count].name = malloc(strlen(name->string_array) + 1);
    if(qname_array[*qname_count].name == NULL) {

        Log_Info("ERROR: Allocating memory failed");
        Array_Free(name);
        free(name);
        return FUNCTION_ERROR;
    }

    strcpy(qname_array[*qname_count].name, name->string_array);
    qname_array[*qname_count].offset = qname_start_offset; // start position in the packet
    (*qname_count)++;
    Array_Free(name);
    free(name);
    return packet_offset - packet_start; 
}

void DNS_Add_Header(unsigned char * packet, Header * header) {

    unsigned short tmp;

    // header id
    tmp = htons(header->id);
    memcpy(packet, &tmp, 2);

    // flags 
    tmp =  (header->flags.qr << 15) | (header->flags.opcode << 11) |
           (header->flags.aa << 10) | (header->flags.tc << 9) |
           (header->flags.rd << 8) | (header->flags.ra << 7) |
           (header->flags.z << 4) | (header->flags.rcode);
    packet[2] = tmp >> 8;
    packet[3] = tmp & 0xFF;

    // qd count  
    tmp = htons(header->qd_count);
    memcpy(packet + 4, &tmp, 2);

    // an count
    tmp = htons(header->an_count);
    memcpy(packet + 6, &tmp, 2);

    // ns count
    tmp = htons(header->ns_count);
    memcpy(packet + 8, &tmp, 2);

    // ar count
    tmp = htons(header->ar_count);
    memcpy(packet + 10, &tmp, 2);
}

int DNS_Add_Question(unsigned char * packet, Question * question, Qname_struc * names, int * names_count) {

    unsigned short tmp;

    int index = 12;  // header takes first 12
    index += Build_QNAME(question->qname, packet, index, names, names_count);
    if (index == FUNCTION_ERROR) {

        return FUNCTION_ERROR;
    }

    // qtype
    tmp = htons(question->qtype);
    memcpy(packet + index, &tmp, 2);
    index += 2;

    // qclass
    tmp = htons(question->qclass);
    memcpy(packet + index, &tmp, 2);
    index += 2;

    return index;
}

int DNS_Add_Reminder(unsigned char * packet, int index, unsigned char * reminder, int reminder_size) {

    if(reminder != NULL && reminder_size > 0) {

        memcpy(packet + index, reminder, reminder_size);
        index += reminder_size;
        return index;
    }
    return FUNCTION_ERROR;  // no reminder
}

int DNS_Create_Packet(unsigned char *packet, Header * header, Question *question, unsigned char * reminder, int reminder_size) {
   
    Qname_struc *names = calloc(50, sizeof(Qname_struc));
    if (names == NULL) {

        Log_Info("ERROR: Allocation memory failed");
        return FUNCTION_ERROR;
    }

    int names_count = 0;
    DNS_Add_Header(packet, header);         // add header to packet
    int index = DNS_Add_Question(packet, question, names, &names_count);        // add question
    if (index == FUNCTION_ERROR) {

        free(names);
        return FUNCTION_ERROR;
    }
    // add reminder to the packet
    int result = DNS_Add_Reminder(packet, index, reminder, reminder_size);
    free(names);
    if(result != FUNCTION_ERROR) return result;
  
    return index; // no reminder 
}

int DNS_Create_Packet_Header_Only(unsigned char *packet, Header * header, unsigned char *reminder, int reminder_size) {
   
    DNS_Add_Header(packet, header);
    return DNS_Add_Reminder(packet, 12, reminder, reminder_size);
}

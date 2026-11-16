// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file dns_packet_parse.c
 * @author David Švancer
 *
 * @brief Implementation of packet parsing.
 */

#include "../include/dns_packet_parse.h"

int DNS_Parse_Header(unsigned char packet[], ssize_t packet_size, Header * header) {

    if (packet_size < 12) {

        Log_Info("ERROR: Malformed packet, Parsing header failed: packet size is less than 12 bytes");
        return FUNCTION_ERROR;
    }

    // header ID
    header->id = (packet[0] << 8) | packet[1];

    // header flags
    short raw_flags = (packet[2] << 8) | packet[3];
    header->flags.qr = (((unsigned) 1 << 15) & raw_flags) >> 15;
    header->flags.opcode = (((unsigned) 15 << 11) & raw_flags) >> 11;
    header->flags.aa = (((unsigned) 1 << 10) & raw_flags) >> 10;
    header->flags.tc = (((unsigned) 1 << 9) & raw_flags) >> 9;
    header->flags.rd = (((unsigned) 1 << 8) & raw_flags) >> 8;
    header->flags.ra = (((unsigned) 1 << 7) & raw_flags) >> 7;
    header->flags.z = (((unsigned) 7 << 4) & raw_flags) >> 4;
    header->flags.rcode = (((unsigned) 15) & raw_flags);

    // qd count
    header->qd_count = (packet[4] << 8) | packet[5];

    // an count
    header->an_count = (packet[6] << 8) | packet[7];

    // ns count
    header->ns_count = (packet[8] << 8) | packet[9];

    // ar count
    header->ar_count = (packet[10] << 8) | packet[11];

    return FUNCTION_SUCCESS;
}

int DNS_Parse_Question(unsigned char packet[], ssize_t packet_size,  Question * question) {

    question->qname = malloc(sizeof(char) * 255);
    if (question->qname == NULL) {

        Log_Info("ERROR: Allocation memory failed");
        return FUNCTION_ERROR;
    }

    // qname
    int offset = DNS_Name_Decompression(packet, question->qname, 12);
    if (offset <= 0 || (offset + 4) > packet_size) {

        Log_Info("ERROR: Malformed packet, Parsing question failed: packet size is too small");
        free(question->qname);
        return FUNCTION_ERROR;
    }

    // save as lowercase 
    for (int i = 0; question->qname[i] != '\0'; i++) {

        question->qname[i] = (char)tolower((unsigned char)question->qname[i]);
    }

    // qtype
    question->qtype = packet[offset] << 8 | packet[offset + 1];

    //qclass
    question->qclass = packet[offset + 2] << 8 | packet[offset+3];
    return offset + 4;
}

int DNS_Parse_Packet(unsigned char *packet, ssize_t packet_size, Header * header, Question * question, unsigned char * reminder, int * reminder_size) {

    int result;
    result = DNS_Parse_Header(packet, packet_size, header);
    if (result == FUNCTION_ERROR) {

        return FUNCTION_ERROR;
    }

    int offset = DNS_Parse_Question(packet, packet_size, question);
    if (offset == FUNCTION_ERROR) {

        return FUNCTION_ERROR;
    }

    DNS_Parse_From(reminder, reminder_size, packet, packet_size, offset);

    return FUNCTION_SUCCESS;
}

bool Is_Packet_Type_Valid(Question * question) {

    // 1 == A, 28 == AAAA
    if (question->qtype == 1 || question->qtype == 28) return true;
    return false;
}

void DNS_Parse_From(unsigned char * reminder, int * reminder_size, unsigned char * packet, int packet_size, int offset) {

    // No reminder
    if(offset >= packet_size) {

        *reminder_size = 0;
    } else {

        *reminder_size = packet_size - offset;
        memcpy(reminder, packet + offset, *reminder_size);
    }
}


int DNS_Parse_Header_Only(unsigned char * packet, ssize_t packet_size, Header * header, unsigned char * reminder, int * reminder_size) {
   
    int result = DNS_Parse_Header(packet, packet_size, header);
    if(result == FUNCTION_ERROR) {

        return FUNCTION_ERROR;
    }

    DNS_Parse_From(reminder, reminder_size, packet, packet_size, 12); // 12 takes header

    return FUNCTION_SUCCESS;
}

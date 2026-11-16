// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file utils.c
 * @author David Švancer
 *
 * @brief Implementation of utils functions. 
 */

#include "../include/utils.h"

int DNS_Name_Decompression(unsigned char packet[], char *decompressed_name, int index) {

    bool jumped = false;
    int index_before_jump = 0;
    int i = index;
    size_t length = 0;
    int result;
    Dynamic_String * name = malloc(sizeof(Dynamic_String));
    if (name == NULL) {

        Log_Info("ERROR: Failed to allocate memory");
        return FUNCTION_ERROR;
    }

    result = Array_Init(name, 20);
    if (result == FUNCTION_ERROR) {

        free(name);
        return FUNCTION_ERROR;
    }

    unsigned char byte;
    while (1) {

        byte = packet[i];
        if (byte == 0) {

            if (!jumped) i++;  
            break;
        }

        if (DNS_Is_Label(byte)) {

            length = DNS_Label_Length(byte);
            i++;
            for (size_t x = 0; x < length; x++) {

                result = Array_AddChar(name, (int) packet[i]);
                if(result == FUNCTION_ERROR) {

                    Array_Free(name);
                    return FUNCTION_ERROR;
                }
                i++;
            }
            result = Array_AddChar(name, (int) '.'); 
            if(result == FUNCTION_ERROR) {

                Array_Free(name);
                return FUNCTION_ERROR;
            }
        } else if (DNS_Is_Pointer(byte)) {

            int offset = DNS_Pointer_Offset(packet[i], packet[i + 1]);
            if (!jumped) {

                index_before_jump = i + 2;
                jumped = true;
            }
            i = offset;
        } else {

            Log_Info("ERROR: Unexpected token while decompressing name");
            Array_Free(name);
            return FUNCTION_ERROR;
            //break;
        }
    }

    Array_Remove_Last_Element(name);
    result = Array_Terminate(name);
    if(result == FUNCTION_ERROR) {

        Array_Free(name);
        return FUNCTION_ERROR;
    }

    strcpy(decompressed_name, name->string_array);    
    Array_Free(name);

    // If we ended with pointer, return position after pointer
    if (jumped) return index_before_jump;    
    return i;
}

bool DNS_Is_Pointer(unsigned char byte) {

    return (byte & 0xC0) == 0xC0;
}

bool DNS_Is_Label(unsigned char byte) {

    return (byte & 0xC0) == 0x00;
}

int DNS_Pointer_Offset(unsigned char char_1, unsigned char char_2) {

    int value = ((char_1 << 8) | char_2);
    // null first 2  (0x3FFF = 0011111111111111)
    value &= 0x3FFF;
    return value;
}

int DNS_Label_Length(unsigned char char_l) {

    return (int) char_l;
}

int Read_File(char * file_name, Node ** list) {

    FILE * file = fopen(file_name, "r");
    if(file == NULL) {

        Log_Info("ERROR: Failed to open file");
        return FUNCTION_ERROR;
    }

    int read_char;
    bool read_flag = true;
    int count_of = 0;
    int result;
    Dynamic_String * line = malloc(sizeof(Dynamic_String));
    if(line == NULL) {

        Log_Info("ERROR: Failed to allocate memory");
        fclose(file);
        return FUNCTION_ERROR;
    }
    result = Array_Init(line, 20);
    if (result == FUNCTION_ERROR) {

        free(line);
        fclose(file);
        return FUNCTION_ERROR;
    }

    while((read_char = fgetc(file)) != EOF) {

        if(isspace(read_char) && (read_char != '\n')) {

            continue;
        }
        if(read_char == '#') {

            read_flag = false;
        }

        if((read_char != '\n') && read_flag) {

            count_of++;
            result = Array_AddChar(line, read_char);
            if (result == FUNCTION_ERROR) break;
        }

        if(read_char == '\n') {

            if (line->string_length != 0) {

                result = Array_Terminate(line);
                if (result == FUNCTION_ERROR) break;

                // Save to lower case
                for (int i = 0; i < line->string_length; i++) {

                    line->string_array[i] = (char)tolower((unsigned char)line->string_array[i]);
                }

                result = Add_To_List(list, line->string_array);
                if (result == FUNCTION_ERROR) break;

                Array_Free(line);

                result = Array_Init(line, 20);
                if (result == FUNCTION_ERROR) break;
                count_of = 0;
            }
            read_flag = true;
        }
    }

    if(count_of>0) {

        result = Array_Terminate(line);
        if (result == FUNCTION_ERROR) {

            Array_Free(line);
            free(line);
            fclose(file);
            return FUNCTION_ERROR;
        }
        result = Add_To_List(list, line->string_array);
        if (result == FUNCTION_ERROR) {
            
            Array_Free(line);
            free(line);
            fclose(file);
            return FUNCTION_ERROR;
        }
    }

    if (result == FUNCTION_ERROR) {

        Array_Free(line);
        free(line);
        fclose(file);
        return FUNCTION_ERROR;
    }

    Array_Free(line);
    free(line);
    fclose(file);
    return FUNCTION_SUCCESS;
}

bool Is_Valid_Domain(Node * domain_list, char * domain) {

    int domain_length = strlen(domain);
    int current_domain_length = 0;                      // lenght of domain name in list
    int length_difference = 0;
    while (domain_list) {

        current_domain_length = strlen(domain_list->hostname);
        if (strcmp(domain, domain_list->hostname) == 0)
        {
            return false;
        }
        if (domain_length > current_domain_length)
        {
            length_difference = domain_length - current_domain_length;
            if(((strcmp(domain + length_difference, domain_list->hostname)) == 0) && 
                domain[length_difference-1] == '.') {

                return false;
            } 
        }
        domain_list = domain_list->next;
    }
  
    return true;
}

void Clean_Memory_And_Exit_Thread(Thread_Arguments * thread_args, Header * header, Question * question) {

    if (thread_args != NULL)  free(thread_args);
    if (header != NULL)       free(header);
    if (question != NULL)     free(question);

    pthread_exit(NULL);
}

void Clean_Memory(Parameters * param, Node ** list, Socket * client_socket, Socket * server_socket) {

    if (param != NULL) {
        if(param->file != NULL) free(param->file);
        if(param->server != NULL) free(param->server);
        free(param);
    }
    if (list != NULL) {

        List_Dispose(list);
        free(list);
    }           
    if (client_socket != NULL)  free(client_socket);
    if (server_socket != NULL)  free(server_socket);
}


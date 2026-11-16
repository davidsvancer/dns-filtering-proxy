// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file log.c
 * @author David Švancer
 *
 * @brief Implementation for loging. 
 */

#include "../include/log.h"

bool verbose_enabled = false; 

void Log_Info(const char *fmt, ...) {
    
    if (!verbose_enabled) return;

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}
    

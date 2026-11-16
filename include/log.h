// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file log.h
 * @author David Švancer
 *
 * @brief Header file for login.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define FUNCTION_ERROR -1
#define FUNCTION_SUCCESS 0
#define FUNCTION_PRINT_HELP 3

extern bool verbose_enabled;

/**
 * @brief Function to log useful helper messages.
 * @param[in] fmt Format string for the message.
 * @param[in] ... Additional arguments.
 */
void Log_Info(const char *fmt, ...);


#endif

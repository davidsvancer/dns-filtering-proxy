// $Author: David Švancer <xsvancd00@stud.fit.vutbr.cz>
/**
 * @file thread.h
 * @author David Švancer
 *
 * @brief Header file for new thread implementation.
 */
#ifndef THREAD_H
#define THREAD_H

#include "dns.h"

/**
 * @brief Function for Handling new thread.
 * @param[in] args Thread arguments.
 */
void * Handle_Request(void *args);

#endif

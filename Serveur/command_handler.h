#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)
#include <sys/types.h>

#else

#error not defined for this platform

#endif

#define CRLF        "\r\n"
#define BUF_SIZE    1024

int handle_command(char* buffer, char** separated_buffer);

static char* get_param(char *buffer);

#endif /* guard */

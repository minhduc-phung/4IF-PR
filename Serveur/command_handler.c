#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "command_handler.h"

int handle_command(char* buffer, char** separated_buffer){
    int i =0;
    for (i=0; i< 3; i++) {
        separated_buffer[i] = malloc(sizeof(char)*BUF_SIZE);
    }
    char* command = malloc(sizeof(char)*BUF_SIZE);
    command = get_param(buffer);
    if ((strcmp(buffer, "msgto") != 0) && (strcmp(buffer, "msgto ") != 0) && (strcmp(command, "msgto") == 0)) {
        char* name_and_message_buffer = buffer + strlen(command) + 1;
        char* name_to_chat = malloc(sizeof(char)*BUF_SIZE);
        name_to_chat = get_param(name_and_message_buffer);

        char* message;

        if (strcmp(name_to_chat, name_and_message_buffer) != 0) {
            message = name_and_message_buffer+ strlen(name_to_chat)+1;
        }else{
            message = "";
        }
        separated_buffer[0] = command;
        separated_buffer[1] = name_to_chat;
        separated_buffer[2] = message;
        return 1;
    // Create a group chat
    } else if ((strcmp(buffer, "crtgr") != 0) && (strcmp(buffer, "crtgr ") != 0) && (strcmp(command, "crtgr") == 0)){
         

    }
}

static char* get_param(char *buffer) {
    char* param = malloc(sizeof(char)*BUF_SIZE);
    int i = 0;
    while(buffer[i] != ' ' && buffer[i] != '\0'){
        param[i] = buffer[i];
        i++;
    }
    param[i] = '\0';
    return param;
}
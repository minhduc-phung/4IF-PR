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
    // Direct Message
    if ((strcmp(buffer, "drmsg") != 0) && (strcmp(buffer, "drmsg ") != 0) && (strcmp(command, "drmsg") == 0)) {
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
    // Create Group
    } else if ((strcmp(buffer, "crtgr") != 0) && (strcmp(buffer, "crtgr ") != 0) && (strcmp(command, "crtgr") == 0)){
        // Get the name of the group
        char* group_and_members = buffer + strlen(command) + 1;
        char* group_name = malloc(sizeof(char)*BUF_SIZE);
        group_name = get_param(group_and_members);
        // Get the members of the group
        char* members = malloc(sizeof(char)*BUF_SIZE);
        if (strcmp(group_name, group_and_members) != 0) {
            members = group_and_members + strlen(group_name) + 1;
        }else{
            members = "";
        }
        separated_buffer[0] = command;
        separated_buffer[1] = group_name;
        separated_buffer[2] = members;
        return 2;
    // Group Message
    } else if ((strcmp(buffer, "grmsg") != 0) && (strcmp(buffer, "grmsg ") != 0) && (strcmp(command, "grmsg") == 0)){
        // Get the name of the group
        char* group_and_message = buffer + strlen(command) + 1;
        char* group_name = malloc(sizeof(char)*BUF_SIZE);
        group_name = get_param(group_and_message);
        // Get the message
        char* message = malloc(sizeof(char)*BUF_SIZE);
        if (strcmp(group_name, group_and_message) != 0) {
            message = group_and_message + strlen(group_name) + 1;
        }else{
            message = "";
        }
        separated_buffer[0] = command;
        separated_buffer[1] = group_name;
        separated_buffer[2] = message;
        return 3;
    }
    return 0;
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
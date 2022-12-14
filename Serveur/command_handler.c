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
    // Direct message
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
    // Create group
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
    // Group message
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
    // Leave group
    else if ((strcmp(buffer, "leavegr") != 0) && (strcmp(buffer, "leavegr ") != 0) && (strcmp(command, "leavegr") == 0)){
        // get the name of the group
        char* group_name = buffer + strlen(command) + 1;
        separated_buffer[0] = command;
        separated_buffer[1] = group_name;
        separated_buffer[2] = "";
        return 4;
    }
    // List groups
    else if (strcmp(command, "lsgr") == 0){
        separated_buffer[0] = command;
        separated_buffer[1] = "";
        separated_buffer[2] = "";
        return 5;
    }
    // Remove group
    else if ((strcmp(buffer, "rmgr") != 0) && (strcmp(buffer, "rmgr ") != 0) && (strcmp(command, "rmgr") == 0)){
        // Get the name of the group
        char* group_name = buffer + strlen(command) + 1;
        separated_buffer[0] = command;
        separated_buffer[1] = group_name;
        separated_buffer[2] = "";
        return 6;
    }
    // List members
    else if ((strcmp(buffer, "lsmem") != 0) && (strcmp(buffer, "lsmem ") != 0) && (strcmp(command, "lsmem") == 0)){
        // Get the name of the group
        char* group_name = buffer + strlen(command) + 1;
        separated_buffer[0] = command;
        separated_buffer[1] = group_name;
        separated_buffer[2] = "";
        return 7;
    }
    else if ((strcmp(buffer, "allmsg") != 0) && (strcmp(buffer, "allmsg ") != 0) && (strcmp(command, "allmsg") == 0)){
		char* message;

        if (strcmp(buffer, command) != 0) {
            message = buffer+ strlen(command)+1;
        }else{
            message = "";
        }

        separated_buffer[0] = command;
        separated_buffer[1] = "";
        separated_buffer[2] = message;
        return 8;
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
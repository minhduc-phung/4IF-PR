# TP 4IF- Programmation Réseaux / Network Programming
## 1. Members of the group
Our group consists of 2 students:
- **PHUNG Minh Duc**
- **PHAM Quoc Viet**
## 2. Description of the project
In the course of the project, we managed to implement the two following features:
- The client can send direct messages to another client.
- The clients can make a group chat and send messages to the group.


## 3. How to use the command-line interface
We decided to use the command-line interface to send commands and messages:
- To send a global message, the client can type anything that does not start with the name of any of the commands and press Enter. (*note that writing only the command without any arguments will send a message with the command name as the content*)
- `drmsg [username] [message]` to send a direct message to another client.
- `grmsg [groupname] [message]` to send a message to a group.
- `crtgr [groupname] [username1] [username2] [username3] ...` to create a group. Note that the creator of the group is automatically added to the group.
- `leavegr [groupname]` to leave a group.
- `lsgr` to list all the groups.
- `rmgr [groupname]` to remove a group. Only the creator of the group can remove it.
- `lsmem [groupname]` to list all the members of a group.

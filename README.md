# TP 4IF - Programmation Réseaux / Network Programming

[English version](#english-version)

## 1. Composition du binôme
- PHUNG Minh Duc
- PHAM Quoc Viet

## 2. Description des fonctionnalités implémentées
Au cours du projet, nous avons réussi à mettre en œuvre les fonctionnalités suivantes :
- Messagerie de diffusion : un client peut envoyer un message à tous les clients connectés au serveur. Tous les clients hors ligne se connectant au serveur verront immédiatement les messages envoyés dans le chat de diffusion.
- Messagerie privé : Un client peut envoyer des messages directs à un autre client.
- Messagerie de groupe : Un client peuvent créer un groupe de discussion et envoyer des messages au groupe.
- Historique des conversations : les messages sont persistants et sont chargés à chaque démarrage du serveur. Les clients qui se connectent au serveur peuvent également voir tous les messages qui leur ont été envoyés lorsqu'ils étaient hors ligne.

## 3. Comment compiler et exécuter les programmes
La compilation et l'exécution de notre programme se fait sur une invite de commande.
Pour compiler le programme, changez le répertoire de travail actuel au dossier du projet et tapez `make`. Deux fichiers exécutables devraient être générés, un pour le serveur et un pour le client.

Pour démarrer le serveur sur une machine, tapez la commande `./server`.

**Remarque** : Si vous tapez l'invite de commande après cela sur la machine serveur, le serveur sera **arrêté** et tous les clients qui s'y connectent seront déconnectés.

Après avoir démarré un serveur, tapez la commande `./client2 [adresse_ip_serveur] [nom_utilisateur]` pour vous connecter au serveur avec le nom d'utilisateur `nom_client`. Ensuite, le client peut taper des commandes ci-dessous pour utiliser le programme :
- Par défaut, une fois le client connecté, pour envoyer un message global, le client peut taper tout ce qui ne commence pas par le nom d'une des commandes et appuyer sur Entrée. (*notez que si vous écrivez seulement la commande sans aucun argument, vous enverrez un message avec le nom de la commande comme contenu*)
- `drmsg [nom_utilisateur] [message]` : Envoie un message direct à un autre client et change la salle de discussion par défaut en salle de discussion privée avec ce client. (c'est-à-dire que toute ligne qui ne commence pas par le nom d'une commande sera comptée comme un message direct à ce client)
- `allmsg [message]` : Envoie un message à tous les clients et change la salle de discussion par défaut en salle de discussion "à tous".
- `grmsg [nom_du_groupe] [message]` : Envoie un message à tous les membres d'un groupe et change la salle de discussion par défaut en salle de discussion de ce groupe.
- `crtgr [nom du groupe] [utilisateur1] {utilisateurN}` : Crée un groupe avec les noms d'utilisateurs saisis comme membres. Notez que le créateur du groupe est automatiquement ajouté au groupe.
- `leavegr [nom du groupe]` : Quitte un groupe.
- `lsgr` : Liste tous les groupes existants.
- `rmgr [nom du groupe]` : Supprime un groupe. Seul le créateur du groupe peut le supprimer.
- `lsmem [nom du groupe]` : Liste tous les membres d'un groupe.


#### English version
## 1. Members of the group
Our group consists of 2 students:
- **PHUNG Minh Duc**
- **PHAM Quoc Viet**

## 2. Description of implemented funcionalities
In the course of the project, we managed to implement the following functionalities:
- Broadcast chat: A client can send a message to all the clients connected to the server. Any offline clients connecting to the server will immediately see the messages sent in the broadcast chat.
- Private chat: The client can send direct messages to another client.
- Group chat: The clients can make a group chat and send messages to the group.
- History of conversations: the messages are persisted and are loaded whenever the server starts. The clients connectting to the server can also see any messages which were sent to them when they were offline.

## 3. How to compile and execute the programs
To compile the program, go to the project folder on the command prompt and type `make`. Two executable files should be generated, one for the server and one for the client.

To start the server on a machine, type the command `./server`. **Note**: typing in the server machine's command prompt **will shut down the server** and disconnect all the clients connecting to it.

After starting a server, type the command `./client2 [server_ip_address] [client_name]` to connect to the server with username `client_name`. Afterwards, the client can type the commands below to use the program:
- By default, once the client is connected, to send a global message, the client can type anything that does not start with the name of any of the commands and press Enter. (*note that writing only the command without any arguments will send a message with the command name as the content*)
- `drmsg [username] [message]`: send a direct message to another client and change the default chating room to "private chatroom" with that client. (i.e. any lines that does not start with the name of any commands will be counted as a direct message to that client)
- `allmsg [message]`: send a broadcast message to all clients and change the default chatting room back to the "all" chat.
- `grmsg [groupname] [message]`: send a message to all the members of a group and change the default chatting room to that group's chatroom.
- `crtgr [groupname] [username1] {usernameN}`: Create a group with the usernames typed as members. Note that the creator of the group is automatically added to the group.
- `leavegr [groupname]`: Leave a group.
- `lsgr`: List all the existing groups.
- `rmgr [groupname]`: Remove a group. Only the creator of the group can remove it.
- `lsmem [groupname]`: List all the members of a group.

#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <ncurses.h>

#include "network.h"

#define CHANNEL "#cruce-devel "
#define PORT 6667
#define HOST_NAME "irc.freenode.net"

#define BUF_SIZE 1024

static int sockfd;

#ifdef DEBUG
FILE *Log;

void printString(char *s) {
    for (int i = 0; i < BUF_SIZE; i++) {
        if (s[i] > ' ')
            fprintf(Log, "%c ", s[i]);
        else
            fprintf(Log, "%d ", s[i]);
    }
    fprintf(Log, "\nEND\n");
}
#endif

void sendIrcMessage(char *message)   //TODO: errors
{
    char messageCommand[strlen(message) + strlen(CHANNEL) + 11];
    sprintf(messageCommand, "PRIVMSG %s:%s\n", CHANNEL, message);
    write(sockfd, messageCommand, strlen(messageCommand));
}

void disconnect(int sockfd)
{
    write(sockfd, "QUIT\n", 5);
    close(sockfd);
}

int Connect(char *name)
{
#ifdef DEBUG
    Log = fopen("Log.txt", "w");
#endif
    int portno = PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;     //TODO: error code
    }
    struct hostent *server = gethostbyname(HOST_NAME);
    if (server == NULL) {
        return -2;     //TODO: error code
    }
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr_list[0], (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        return -3; //TODO: error code
    }

    char nickCommand[20];
    char joinCommand[20];
    char userCommand[20];

    sprintf(nickCommand, "NICK %s\n", name);
    sprintf(userCommand, "USER %s 8 * :%s\n", name, name);
    sprintf(joinCommand, "JOIN %s\n", CHANNEL);

    write(sockfd, "PASS *\n", 7);
    write(sockfd, nickCommand, strlen(nickCommand));
    write(sockfd, userCommand, strlen(userCommand));
    write(sockfd, joinCommand, strlen(joinCommand));

    return sockfd;
}

void *readFromSocket(void *arg)
{
    char buffer[BUF_SIZE];
    char line[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    int j = 0;
    while(1) {
        int n = read(sockfd, buffer, BUF_SIZE);
        buffer[n] = '\0';
        for (int i = 0; i < BUF_SIZE && buffer[i] != '\0'; i++, j++) {
            line[j] = buffer[i];
            if (buffer[i] == '\n') {
                line [j + 1] = '\0';
                ircParse(line, arg);
                j = -1;
            }
        }
    }
    return NULL;
}

void *readFromKeyboard(void *arg)
{
    char buffer[BUF_SIZE];
    while(1) {
        int n = wscanw(arg, "%[^\n]", buffer);
        sendIrcMessage(buffer);
    }
    return NULL;
}

struct Message *newMessage(int prefixLen, int commandLen, int trailingLen)
{
    struct Message *message = malloc(sizeof(struct Message));
    message->prefix   = malloc(prefixLen);
    message->command  = malloc(commandLen);
    message->trailing = malloc(trailingLen);
    return message;
}

void distroyMessage(struct Message **message)
{
    free((*message)->prefix);
    free((*message)->command);
    free((*message)->trailing);
    free( *message);
    *message = NULL;
}

struct Message *ircParse(char *str)
{
    char *p;
    while ((p = strchr(str, '\r'))) {
        *p = ' ';
    }

    char *prefixEnd     = str + 1; //parsing prefix
    int prefixLen       = 0;
    if (str[0] == ':') {
        prefixEnd       = strchr(str, ' ') - 1;
        prefixLen       = prefixEnd - str;
    }

    char *trailingStart = strchr(prefixEnd + 2, ' ') + 1; //parsing trailing
    int   trailingLen   = strlen(trailingStart);

    char *commandStart  = prefixEnd + 2; //parsing command
    char *commandEnd    = trailingStart - 2;
    int   commandLen    = commandEnd - commandStart + 1;

    struct Message *message = newMessage(prefixLen   + 1,
                                         commandLen  + 1,
                                         trailingLen + 1);

    strncpy(message->prefix, str+1, prefixLen);
    strncpy(message->command, commandStart, commandLen);
    strncpy(message->trailing, trailingStart, trailingLen);

    message->prefix[prefixLen]     = '\0';
    message->command[commandLen]   = '\0';
    message->trailing[trailingLen] = '\0';

    return message;
}

void handleMessage(struct Message *message, void *win)
{
    wprintw(win, "%s\n", message->prefix);
    wprintw(win, "%s\n", message->command);
    wprintw(win, "%s", message->trailing);
    wrefresh(win);
}


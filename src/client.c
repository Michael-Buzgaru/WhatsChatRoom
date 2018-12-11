#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "prototypes.h"
#include "memory.h"
#include "client.h"

void set_client( struct node *const info_client, struct sockaddr_in *info_serv, const char *const userName )
{
    info_client->prev = current;
    current->next = info_client;
    current = info_client;
    strcpy( info_client->userName, userName );
    strcpy( info_client->ip, inet_ntoa( info_serv->sin_addr ) );
    info_client->port = ntohs( info_serv->sin_port );
}

void client_join_chat( const struct node *const info_client )
{
    printf("*** %s[%d]\tIP(%s:%d)\t\tjoin to Chat. ***\n", info_client->userName , info_client->clientID , info_client->ip , info_client->port );
}

void client_leave ( const int client_ID )
{
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        if ( tmp->clientID == client_ID )
        {
            printf("*** %s[%d]\tIP(%s:%d)\t\tLeave the ChatRoom ***\n",tmp->userName ,tmp->clientID, tmp->ip, tmp->port );
            removeNode( client_ID, &root );
            close( client_ID );
            break;
        }
        tmp = tmp->next;
    }
}

void client_join_and_leave ( const char *const clientIP, const int clientPort, const int clientID )
{
    printf( "Client(%s:%d) - ID(%d) Join/Leave\n", clientIP, clientPort, clientID );
    //rem_client_from_list( clientID );
}

void rem_client_from_list( const int clientID )
{
    (void)clientID;
    for ( int i = 0 ; i < max_clients ; i++ )
    {
        printf("cli = %d\n", max_list_clients[ i ] );
    }
}

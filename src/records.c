#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "prototypes.h"
#include "records.h"
#include "messages.h"
#include "memory.h"

int search_client_in_list ( const int client_ID )
{
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        if ( tmp->clientID == client_ID )
        {
            return tmp->clientID;
        }
        tmp = tmp->next;
    }

    return 0;
}

char *search_userName_in_list ( const int client_ID )
{
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        if ( tmp->clientID == client_ID )
        {
            return tmp->userName;
        }
        tmp = tmp->next;
    }

    return NULL;
}

int findID ( const int id )
{
    for ( int i = 0 ; i < max_clients ; i++ )
    {
        if ( id == max_list_clients[i] )
        {
            printf("Found\n");
            return 1;
        }
    }
    return 0;
}

void onlineUsers ( const int clientID )
{
    int online_clients = 0;
    size_t len = getListLength( &online_clients );
    char *user_list[ online_clients ];
    int i = get_online_users( user_list );

    const char *online_users = "\n\t*** Online Users: ***\n\t\t";
    const char *online_users2 = "\t\t";

    len = len + strlen( online_users ) + strlen( online_users2 ) + strlen( "\n" );
    char *snd_msg = user_calloc( &snd_msg, len );
    if (  snd_msg )
    {
        strcpy( snd_msg, online_users );
        for ( int j = 0 ; j < i ; j++ )
        {
            strcat ( snd_msg, user_list[j] );
            strcat ( snd_msg, "\n" );
            strcat ( snd_msg, online_users2 );
        }
        send( clientID, snd_msg, strlen( snd_msg ), 0 );
    }

    if ( snd_msg )
    {
        free ( snd_msg );
    }
}

size_t getListLength ( int *const clients_online )
{
    size_t len = 0;
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        len += strlen( tmp->userName );
        (*clients_online)++;
        tmp = tmp->next;
    }
    return len;
}

int get_online_users( char **users )
{
    int ret = 0;
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        users[ ret ] = tmp->userName;
        ret++;
        tmp = tmp->next;
    }
    return ret;
}

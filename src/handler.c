#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prototypes.h"
#include "handler.h"
#include "messages.h"
#include "memory.h"

struct node *client_node( const int clientId, const char *const client_ip )
{
    struct node *client = malloc( sizeof( struct node ) );
    client->clientID = clientId;
    client->prev = NULL;
    client->next = NULL;
    strcpy( client->ip, client_ip );
    strcpy( client->userName, "NULL" );
    return client;
}


size_t get_length_null_char( const char *const src )
{
    if ( check_ptr( src, "(- SRC - )in get_length_null_char" ) )
    {
        size_t len_src = strlen( src );
        if ( len_src > 0 )
        {
            if ( DEBUG )
            {
                printf( "\n\t*** get_length_null_char() OK. ***\n" );
            }
            return len_src + 1;
        }
    }

    printf( "\n*** get_length_null_char() return 0 ***\n" );
    return 0;
}

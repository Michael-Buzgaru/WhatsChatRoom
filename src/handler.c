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

char *src_to_dest ( char *const dest, const size_t len_dest, const char *const src, const size_t len_src )
{
    int flag = 0;
    if ( check_ptr ( src, "(- SRC - )in src_to_dest") == NULL )
    {
        flag = 1;
    }

    if ( check_ptr ( dest, "(- DEST - ) in src_to_dest") == NULL)
    {
        flag = 1;
    }

    if ( len_src < 1  )
    {
        printf("*** Len_src in src_to_dest = %zu ***\n", len_src );
        flag = 1;
    }

    if ( len_dest <  1 )
    {
        printf("*** Len_dest in src_to_dest = %zu ***\n", len_dest );
        flag = 1 ;
    }

    if ( flag == 1 )
    {
        printf( "\n*** src_to_dest() return NULL ***\n" );
        return NULL;
    }

    if ( DEBUG )
    {
        printf( "\n\t*** src_to_dest() OK. ***\n");
    }
    return strcpy( dest, src );
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

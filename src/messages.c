#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "prototypes.h"
#include "handler.h"
#include "messages.h"
#include "records.h"
#include "memory.h"

ssize_t send_sckt ( const int clientID, char *const msg )
{
    if ( findID( clientID) == 0 )
    {
        if ( ( msg != NULL ) || ( *msg != '\0' ) || ( strcmp( msg, "") != 0 ) || ( *msg != '\n' ) )
        {
            size_t len = strlen( msg );
            ssize_t ret = send( clientID, msg, len , 0 );

            if ( ret == -1 )
            {
                printf( "Error, send()\n" );
                fprintf( stderr, "send: %s (%d)\n", strerror( errno ), errno );
                printf("ClientID = %d\n", clientID );
                exit( EXIT_FAILURE );
            }
            return ret;
        }
    }

    return -1;
}

ssize_t recv_sckt ( const int clientID, char *msg )
{
    char buffer[ MAX_LENGTH_RECV ];
    mem_set_empty_buffer( buffer, MAX_LENGTH_RECV );
    ssize_t recv_ret = recv( clientID, buffer, MAX_LENGTH_RECV - 1, 0 );
    if ( recv_ret == -1 )
    {
        printf( "Error, recv()\n" );
        fprintf( stderr, "recv: %s (%d)\n", strerror( errno ), errno );
        return -1;
    }else if ( recv_ret <  MAX_LENGTH_RECV )
    {
        buffer[strcspn(buffer, "\n")] = 0;
        if ( strlen( buffer ) > 0 )
        {
            strcpy( msg, buffer );
        }
    }else if ( recv_ret >= MAX_LENGTH_RECV )
    {
        printf("recv Overflow\n");
        close( clientID );
        return -1;
    }

    return recv_ret;
}

void send_public_msg( const int clientID, char *msg )
{
    char *name = search_userName_in_list ( clientID );
    int pub_msg = 0;
    struct node *tmp = root->next;
    while ( tmp != NULL)
    {
        if ( tmp->clientID != clientID )
        {
            if (  pub_msg == 0 )
            {
                //printf( "(%s)%s\n", tmp->ip, msg );
                pub_msg = 1;
            }
            size_t len = strlen( name ) + strlen( msg );
            char buffer[ len ];
            mem_set_empty_buffer( buffer, len );
            sprintf( buffer, "%s: %s",name, msg );
            printf("%s\n", buffer );
            send_sckt ( tmp->clientID, buffer );
        }
        tmp = tmp->next;
    }
    pub_msg = 0;
}

void print_msg( const char *const where, const char *const src )
{
    printf( "\n\t*** Error, in %s(), %s. ***\n\n", where, src );
}


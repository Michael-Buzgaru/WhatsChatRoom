#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "prototypes.h"
#include "messages.h"
#include "records.h"

ssize_t send_sckt ( const int socketID, char *const msg )
{
    if ( ( msg != NULL ) | ( *msg != '\0' ) | ( strcmp( msg, "") != 0 )| ( *msg != '\n' ) )
    {
        size_t len = strlen( msg );
        ssize_t send_ret = send( socketID, msg, len , 0 );
        if ( send_ret == -1 )
        {
            printf( "Error, send()\n" );
            fprintf( stderr, "send: %s (%d)\n", strerror( errno ), errno );
            exit( EXIT_FAILURE );
        }
        return send_ret;
    }
    return -1;
}

ssize_t recv_sckt ( const int clientID, char *msg )
{
    char buffer[MAX_LENGTH_MSG] = { 0 };
    ssize_t recv_ret = recv( clientID, buffer, MAX_LENGTH_MSG - 1, 0 );
    if ( recv_ret == -1 )
    {
        printf( "Error, recv()\n" );
        fprintf( stderr, "recv: %s (%d)\n", strerror( errno ), errno );
        return -1;
    }else if ( recv_ret < ( MAX_LENGTH_MSG ) )
    {
        buffer[recv_ret] = '\0';
        buffer[ strcspn( buffer, "\n" ) ] = 0;
        strcpy( msg, buffer );
    }else if ( recv_ret >= MAX_LENGTH_MSG )
    {
        printf("recv Overflow\n");
        close( clientID );
        return -1;
    }

    return recv_ret;
}

void print_msg( const char *const where, const char *const src )
{
    printf( "\n\t*** Error, in %s(), %s. ***\n\n", where, src );
}

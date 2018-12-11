#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prototypes.h"
#include "instance.h"

static char *program_path = NULL;

void block_new_instance( void )
{
    struct flock file_lock;
    char *dir = getenv( "HOME" );
    if ( dir == NULL || dir[0] != '/' )
    {
        fprintf( stderr, "Wrong Directory, getenv(): %s (%d)\n", strerror( errno ), errno );
        exit( EXIT_FAILURE );
    }
    program_path = calloc( sizeof ( *program_path ), strlen( dir ) + sizeof( "/" "serv" ) );
    if ( program_path == NULL )
    {
        printf( "Error, malloc()\n" );
        exit ( EXIT_FAILURE );
    }else
    {
        memcpy( program_path, dir, strlen( dir ) );
        memcpy( program_path + strlen( dir ), "/" "serv", sizeof( "/" "serv" ) );
    }

    int file_desk = open( program_path, O_RDWR | O_CREAT, 0600 );
    if ( file_desk < 0 )
    {
        fprintf( stderr, "open: %s (%d)\n", strerror( errno ), errno );
        exit( EXIT_FAILURE );
    }

    file_lock.l_start   = 0;
    file_lock.l_len     = 0;
    file_lock.l_type    = F_WRLCK;
    file_lock.l_whence  = SEEK_SET;

    if ( fcntl( file_desk, F_SETLK, &file_lock ) < 0 )
    {
        fprintf( stderr, "Server is already running\n" );
        exit( EXIT_FAILURE );
    }
    atexit( clear_link );
}

void clear_link( void )
{
    unlink( program_path );
    free ( program_path );
}

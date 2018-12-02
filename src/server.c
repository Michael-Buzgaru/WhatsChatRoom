#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include "prototypes.h"
#include "server.h"
#include "handler.h"

void getIP( void )
{
    struct sockaddr_in serv;
    struct sockaddr_in name;
    const char *serv_ip = NULL;

    socklen_t namelen = sizeof( name );
    int sock = socket ( AF_INET, SOCK_DGRAM, 0 );
    if ( sock == -1 )
    {
        printf( "Error, socket()\n" );
        fprintf( stderr, "socket: %s (%d)\n", strerror( errno ), errno );
        exit( EXIT_FAILURE );
    }
    memset( &serv, 0, sizeof( serv ) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( DNS_SERVER_CHECK );
    serv.sin_port = htons( PORT );
    connect( sock , ( const struct sockaddr* ) &serv , sizeof( serv ) );
    getsockname( sock, ( struct sockaddr* ) &name, &namelen );
    if ( ( serv_ip = inet_ntop( AF_INET, &name.sin_addr, host_ip, 100 ) ) == NULL )
    {
        printf ( "Error number : %d . Error message : %s \n" , errno , strerror( errno ) );
        close( sock );
        return;
    }
    if ( DEBBUG )
    {
        printf( "IP = %s - Port = %d\n", host_ip, PORT );
    }
    close( sock );
}

void getPort ( const char *const find_port )
{
    port = (uint16_t)atoi( find_port );
}

int create_socket ( struct sockaddr_in *const info, const size_t server_addrlen )
{
    getIP();
    int option = 0;
    //struct sockaddr_in *server_info = (struct sockaddr_in *)ptr_server_info;
    int ret = socket( AF_INET , SOCK_STREAM , 0 );
    if ( ret == -1 )
    {
        printf( "Error, socket()\n" );
        fprintf( stderr, "socket: %s (%d)\n", strerror( errno ), errno );
        exit( EXIT_FAILURE );
    }
    if( setsockopt( ret, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof( option ) ) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    memset( info, 0, server_addrlen );
    info->sin_family      = AF_INET;
    info->sin_addr.s_addr = inet_addr( host_ip );
    info->sin_port        = htons( port );
    if ( DEBBUG )
    {
        printf( "socket() \t\tOK\n" );
    }
    return ret;
}

int bind_sckt( const struct sockaddr_in *const info, const size_t server_addrlen )
{
    int ret = bind( server_sckt, ( struct sockaddr * )info, ( socklen_t )server_addrlen );
    if ( ret == -1 )
    {
        printf( "Error, bind()\n" );
        fprintf( stderr, "bind: %s (%d)\n", strerror( errno ), errno);
        exit ( EXIT_FAILURE );
    }
    if ( DEBBUG )
    {
        printf( "bind() \t\tOK\n");
    }
    return ret;
}

int list_sckt( void )
{
    int ret = listen( server_sckt, MAX_PENDING_CONNECTIONS );
    if ( ret == -1 )
    {
        printf( "Error, listen()\n" );
        fprintf( stderr, "listen: %s (%d)\n", strerror( errno ), errno );
        exit ( EXIT_FAILURE );
    }
    if ( DEBBUG )
    {
        printf( "listen() \tOK\n" );
    }
    return ret;
}

int accept_sckt ( const struct sockaddr_in *const client_info, const size_t *c_addrlen )
{
    int ret = accept( server_sckt, (struct sockaddr*) client_info, (socklen_t*) c_addrlen);
    if ( ret == -1 )
    {
        printf( "Error, accept()\n" );
        fprintf( stderr, "accept: %s (%d)\n", strerror( errno ), errno );
        exit ( EXIT_FAILURE );
    }
    if ( DEBBUG )
    {
        printf( "accept() \tOK\n" );
    }
    return ret;
}

void set_main_sckt ( int *const sckt_desk, const int *const client_socket_max, fd_set *const readfds )
{
    for ( int i = 0 ; i < max_clients ; i++ )
    {
        *sckt_desk = client_socket_max[i]; ///add new Client
        if( *sckt_desk > 0 )
        {
            FD_SET( *sckt_desk , readfds ); /// if valid Socket add it
        }
        if( *sckt_desk > max_sd )
        {
            max_sd = *sckt_desk; ///set the high number to inform Select()
        }
    }
}

int select_sckt ( fd_set *const readfds )
{
    int activity = select( max_sd + 1 , readfds , NULL , NULL , NULL);
    if ( ( activity < 0 ) && ( errno != EINTR ) )
    {
        printf("select error");
        exit ( EXIT_FAILURE );
    }else
    {
        return activity;
    }
}

int getpeername_sckt ( const int sckt, const struct sockaddr_in *const info, const size_t *length )
{
    int ret = getpeername( sckt, ( struct sockaddr*)info, ( socklen_t* ) length );
    if ( ret == -1 )
    {
        printf( "Error, getpeername()\n" );
        fprintf( stderr, "getpeername: %s (%d)\n", strerror( errno ), errno );
        exit ( EXIT_FAILURE );
    }
    if ( DEBBUG )
    {
        printf( "getpeername() \tOK\n" );
    }

    return ret;
}

void add_client_to_list( const int clientSocket, int *const client_socket_max  )
{
    for ( int i = 0 ; i < max_clients ; i++ )
    { //add new socket to array of sockets
        if( client_socket_max[i] == 0 )
        {    //if position is empty
            client_socket_max[i] = clientSocket;
            printf("****Adding to list socket[%d] as %d\n\n",client_socket_max[i] , i );
            break;
        }
    }
}

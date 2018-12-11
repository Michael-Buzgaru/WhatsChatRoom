#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "prototypes.h"
#include "handler.h"
#include "server.h"
#include "messages.h"
#include "records.h"
#include "memory.h"
#include "instance.h"
#include "client.h"

struct node *root;
struct node *current;

int server_sckt;
char host_ip[ HOST_IP_LEN ];
int max_list_clients[ MAX_SOCKT ];
int max_clients = MAX_SOCKT;
int max_sd;
int flag;
uint16_t port;

int main( int argc, char *argv[] )
{
    block_new_instance();
    signal( SIGINT, catch_ctrl_c_and_exit );
    if (2 != argc)
    {
        fprintf( stderr, "Usage: %s <port>\n", argv[0] );
        exit(1);
    }
    struct sockaddr_in server;
    size_t server_addr_len = sizeof ( server );
    char recive[ MAX_LENGTH_RECV ];
    struct node *new_Client = NULL;
    int client_sckt = 0;
    int sckt_desk;
    ssize_t valread;
    fd_set readfds;

    clear_arr ( max_list_clients );
    getPort( argv[1] );
    server_sckt = create_socket ( &server, server_addr_len );/// Create Server
    *max_list_clients = server_sckt;
    bind_sckt( &server, server_addr_len ); /// Bind the socket to IP() - port 8888
    list_sckt();
    root = client_node( server_sckt, inet_ntoa( server.sin_addr ) );
    current =  root;

    printf( "\n\n\t*** Starting Server[%d] on IP(%s) Port(%d)... ***\n\n", max_list_clients[0], host_ip, port );

   while( TRUE )
    {
        //int tmp_sd = 0;
        FD_ZERO( &readfds ); ///clear the socket set
        FD_SET( server_sckt, &readfds ); ///add master socket to set
        max_sd = server_sckt;

        set_main_sckt( &sckt_desk, &readfds );
        select_sckt( &readfds );

        if (FD_ISSET( server_sckt, &readfds ) )/// Check if there is an incomming connection
        {
            char tmp_userName[MAX_LENGTH_NAME - 1 ] = { 0 };
            client_sckt = accept_sckt( &server, &server_addr_len ); /// We have a connection
            send( client_sckt, "Hello Client", strlen( "Hello CLient"), 0 );
            ssize_t ret = recv_sckt ( client_sckt, tmp_userName );
            if ( ret == 0 )
            {
                client_join_and_leave( inet_ntoa( server.sin_addr ), ntohs( server.sin_port ), client_sckt );
                flag = 1;
            }else
            {
                flag = 0;
                new_Client = client_node( client_sckt, inet_ntoa( server.sin_addr ) );
                set_client( new_Client, &server, tmp_userName );
                client_join_chat ( new_Client );
                add_client_to_list ( client_sckt ); ///add new socket to array of sockets
                onlineUsers( new_Client->clientID );
            }
        }

        if ( flag == 1 )
        {
            for ( int i = 1; i < max_clients; i++ ) /// else its some IO operation on some other socket :)
            {
                sckt_desk = max_list_clients[ i ];
                if ( FD_ISSET( sckt_desk , &readfds ) )
                {
                    mem_set_empty_buffer ( recive, MAX_LENGTH_RECV );
                    if ( ( valread = read( sckt_desk , recive, MAX_LENGTH_RECV ) ) == 0 ) /// Check if Client leave
                    {
                        int client_id = search_client_in_list ( sckt_desk );///  leaves...
                        if ( client_id != 0 )
                        {
                            getpeername( client_id , ( struct sockaddr* )&server , ( socklen_t* )&server_addr_len );
                            client_leave ( client_id ); /// Client disconnected
                            max_list_clients[i] = 0; /// remove from list
                            break;
                        }
                    }
                    else
                    {
                        //send( sckt_desk, recive, strlen( recive ), 0 );
                        //send_public_msg ( sckt_desk, recive );
                    }
                }
            }
        }
    }

    freeList( root );
}

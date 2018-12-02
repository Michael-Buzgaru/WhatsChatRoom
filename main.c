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

struct node;
int server_sckt;
char host_ip[ HOST_IP_LEN ];
struct node *root;
struct node *current;
int max_clients = MAX_SOCKT;
int max_sd;
uint16_t port;

int main( int argc, char *argv[] )
{
    signal( SIGINT, catch_ctrl_c_and_exit );
    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in server;
    size_t server_addr_len = sizeof ( server );
    char recive[ MAX_LENGTH_MSG ];
    struct node *new_Client = NULL;
    int client_sckt , max_list_clients[30], sckt_desk;
    ssize_t valread;
    fd_set readfds;

    clear_arr ( max_list_clients );
    getPort( argv[1] );
    server_sckt = create_socket ( &server, server_addr_len );/// Create Server
    bind_sckt( &server, server_addr_len ); /// Bind the socket to IP() - port 8888
    list_sckt();
    root = client_node( server_sckt, inet_ntoa( server.sin_addr ) );
    current =  root;

    printf("*** Starting Server on IP(%s) Port(%d)... ***\n", host_ip, port);

   while( TRUE )
    {

        FD_ZERO(&readfds); ///clear the socket set
        FD_SET(server_sckt, &readfds); ///add master socket to set
        max_sd = server_sckt;
        set_main_sckt( &sckt_desk, max_list_clients, &readfds);
        select_sckt( &readfds );

        if (FD_ISSET(server_sckt, &readfds))/// Check if there is an incomming connection
        {
            client_sckt = accept_sckt( &server, &server_addr_len ); /// We have a connection
            new_Client = client_node( client_sckt, inet_ntoa( server.sin_addr ) );
            new_Client->prev = current;
            current->next = new_Client;
            current = new_Client;
            ssize_t ret = recv_sckt ( client_sckt, new_Client->userName );
            if ( ret )
            {
                strcpy( new_Client->ip, inet_ntoa( server.sin_addr ));
                new_Client->port = ntohs( server.sin_port );
                client_join_chat ( new_Client );
                add_client_to_list ( client_sckt, max_list_clients ); ///add new socket to array of sockets
            }
        }

        for ( int i = 0; i < max_clients; i++) /// else its some IO operation on some other socket :)
        {
            sckt_desk = max_list_clients[i];
            if (FD_ISSET( sckt_desk , &readfds))
            {
                if ((valread = read( sckt_desk , recive, MAX_LENGTH_MSG)) == 0) /// Check if Client leave
                {
                    int client_id = search_client_in_list ( sckt_desk );///  leaves...
                    if ( client_id != 0 )
                    {
                        getpeername( client_id , ( struct sockaddr* )&server , ( socklen_t* )&server_addr_len );
                        client_leave ( client_id ); /// Client disconnected
                        close( client_id );
                        max_list_clients[i] = 0; /// remove from list
                    }
                }
                else
                {
                    recive[ valread ] = '\0';
                    send( sckt_desk , "hello" , strlen( "hello" ) , 0 );
                }
            }
        }
    }

    freeList( root );
}

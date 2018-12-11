#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

void getIP( void );
void getPort ( const char *const find_port );
int create_socket ( struct sockaddr_in *const info, const size_t server_addrlen );
int bind_sckt     ( const struct sockaddr_in *const info, const size_t server_addrlen );
int list_sckt( void );
int accept_sckt ( const struct sockaddr_in *const client_info, const size_t *c_addrlen );
void set_main_sckt ( int *const sckt_desk, fd_set *const readfds );
int select_sckt ( fd_set *const readfds );
int getpeername_sckt ( const int sckt, const struct sockaddr_in *const info, const size_t *length );
void add_client_to_list( const int clientSocket );

#endif // SERVER_H_INCLUDED

#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

void client_join_chat( const struct node *const info_client );
void client_leave ( const int client_ID );
void client_join_and_leave ( const char *const clientIP, const int clientPort, const int clientID );
void rem_client_from_list( const int clientID );
void set_client( struct node *const info_client, struct sockaddr_in *info_serv, const char *const userName );

#endif // CLIENT_H_INCLUDED

#ifndef RECORDS_H_INCLUDED
#define RECORDS_H_INCLUDED

int search_client_in_list ( const int client_ID );
char *search_userName_in_list ( const int client_ID );
int findID ( const int id );
void removeCliend ( const int id );
void onlineUsers ( const int clientID );
size_t getListLength ( int *const clients_online );
int get_online_users( char **users );

#endif // RECORDS_H_INCLUDED

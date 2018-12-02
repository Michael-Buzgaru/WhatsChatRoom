#ifndef RECORDS_H_INCLUDED
#define RECORDS_H_INCLUDED

void client_join_chat( const struct node *const info_client );
void client_leave ( const int client_ID );
void print_online_users ( const int client_ID );
int search_client_in_list ( const int client_ID );

#endif // RECORDS_H_INCLUDED

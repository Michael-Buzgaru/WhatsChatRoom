#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "prototypes.h"
#include "records.h"
#include "messages.h"
#include "memory.h"

int search_client_in_list ( const int client_ID )
{
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        if ( tmp->clientID == client_ID )
        {
            return tmp->clientID;
        }
        tmp = tmp->next;
    }

    return 0;
}

void client_join_chat( const struct node *const info_client )
{
    print_online_users( info_client->clientID );
    printf("*** %s[%d] - IP(%s) - Port(%d) join to Chat. ***\n", info_client->userName , info_client->clientID , info_client->ip , info_client->port );
}

void client_leave ( const int client_ID )
{
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        if ( tmp->clientID == client_ID )
        {
            printf("Host disconnected , ip %s , port %d \n" , tmp->ip , tmp->port );
            break;
        }
        tmp = tmp->next;
    }
}

void print_online_users ( const int client_ID )
{
    int flag = 0;
    struct node *tmp = root->next;
    while ( tmp != NULL )
    {
        if ( tmp->clientID != client_ID )
        {
            size_t len = strlen( tmp->userName ) + 1;
            const char *ptr_1 = "\t*** Online Users: ***\n\t\t%s\n";
            const char *ptr_2 = "\t\t%s\n";
            size_t len_ptr_1 = strlen( ptr_1 );
            size_t len_ptr_2 = strlen( ptr_2 );

            len = len + len_ptr_1 + len_ptr_2;
            char local_buffer_1[ len ];
            mem_set_empty_buffer( local_buffer_1, len );

            if ( flag == 0 )
            {
                sprintf( local_buffer_1, ptr_1, tmp->userName );
                flag = 1;
            }else
{
                sprintf( local_buffer_1, ptr_2, tmp->userName );
            }
            send_sckt( client_ID, local_buffer_1 );
        }
        tmp = tmp->next;
    }
}

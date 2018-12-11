#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prototypes.h"
#include "memory.h"

const char *check_ptr ( const char *const src, const char *const where )
{
    if ( src == NULL )
    {
        printf( "\n*** The pointer %s() is NULL. ***\n", where );
        return NULL;
    }
    if ( DEBUG )
    {
        printf( "\n*** The pointer %s(), OK. ***\n", where );
    }
    return src;
}

void clear_arr ( int *const arr )
{
    for ( int i = 0 ; i < MAX_SOCKT ; i++ )
    {
        arr[ i ] = 0;
    }
}

char *mem_set_empty_buffer  ( char *const dest, const size_t dest_len )
{
    if ( check_ptr( dest, "(- DEST -)in mem_set_empty_buffer" ) == NULL ){
        return NULL;
    }

    if ( DEBUG )
    {
        printf( "\n\t*** mem_set_empty_buffer() OK ***\n" );
    }
    return memset( dest, '\0', dest_len );
}

void catch_ctrl_c_and_exit( int signal )
{
    struct node *current_local;
    while ( root != NULL )
    {
        if ( root->clientID == server_sckt )
        {
            printf( "\n\n\t*** Server[%d] is Shutting Down[SIG = %d] ***\n\n", root->clientID, signal );
        }else{
            printf( "\nClose socketfd: %d\n", root->clientID );
        }
        close( root->clientID );
        current_local = root;
        root = root->next;
        free( current_local );
    }
    exit( EXIT_SUCCESS );
}

void freeList ( struct node* buffer )
{
    struct node *ret = buffer;
    if ( ret )
    {
        while ( ret != NULL )
        {
            struct node *tmp = ret->next;
            free ( ret );
             ret = tmp;
        }
    }else
    {
        printf("Error, NULL Pointer detected ->> freeList()\n");
        exit ( EXIT_FAILURE);
    }
}

void removeNode ( const int clientID, struct node **head )
{
    struct node *tmp  = *head;
    struct node *prev = NULL;

   while ( tmp != NULL )
   {
      struct node *next = tmp->next;

      if ( tmp->clientID == clientID )
      {
         free( tmp );

         if ( prev )
         {
            prev->next = next;
         }
         else
         {
            *head = next;
         }
      }
      else
      {
         prev = tmp;
      }

      tmp = next;
   }
}

char *user_calloc ( char **buffer, size_t n )
{
    *buffer = calloc( sizeof ( *buffer), n );
    if ( buffer == NULL )
    {
        printf("Error, calloc() in user_calloc()\n" );
        return NULL;
    }

    return *buffer;
}

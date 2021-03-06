#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

void catch_ctrl_c_and_exit( int signal );
const char *check_ptr       ( const char *const src, const char *const where );
void clear_arr ( int *const arr );
void removeNode ( const int clientID, struct node **head );
void freeList ( struct node* buffer );
char *mem_set_empty_buffer  ( char *const dest, const size_t dest_len );
char *user_calloc ( char **buffer, size_t n );

#endif // MEMORY_H_INCLUDED

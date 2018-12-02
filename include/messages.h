#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

ssize_t send_sckt   ( const int socketID, char *const msg );
ssize_t recv_sckt ( const int clientID, char *msg );
void print_msg      ( const char *const where, const char *const src );

#endif // MESSAGES_H_INCLUDED

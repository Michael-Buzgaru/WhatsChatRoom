#ifndef PROTOTYPES_H_INCLUDED
#define PROTOTYPES_H_INCLUDED

#define DNS_SERVER_CHECK "8.8.8.8"
#define DEBBUG 0
#define PORT 8888
#define HOST_IP_LEN 16
#define MAX_LENGTH_NAME 25
#define MAX_LENGTH_RECV 9999
#define MAX_LENGTH_SEND 201
#define BUFFSIZE 6500
#define DEBUG 0
#define MAX_SOCKT 30
#define MAX_PENDING_CONNECTIONS 5
#define TRUE   1
#define FALSE  0


extern char host_ip[ HOST_IP_LEN ];
extern int max_list_clients[ MAX_SOCKT ];
extern int server_sckt;
extern int max_clients;
extern int max_sd;
extern uint16_t port;
extern int flag;

struct node
{
    int clientID;
    struct node* prev;
    struct node* next;
    char ip[ HOST_IP_LEN ];
    int port;
    char userName[ MAX_LENGTH_NAME ];
} node;
struct node *client_node( const int clientId, const char *const client_ip );
extern struct node *root;
extern struct node *current;

#endif // PROTOTYPES_H_INCLUDED

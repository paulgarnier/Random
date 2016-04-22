#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* atoi */
#include <unistd.h> /* setuid, getuid, close */

#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h> /* getaddrinfo */
#include <sys/time.h>

static void usage()
{
    fprintf(stderr,"ping_udp <host> <port>\n");
    exit(-1);
}

int main(int argc, char * argv[])
{
    struct addrinfo hints, *res, *res0;

    unsigned long port;
    char * dummy;
    const char *cause = NULL;

    int s, error;

    struct timeval tv_tmp;
    struct timeval timestamp_send;
    struct timeval timestamp_return;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    int send_data[8];
    int recv_data_len=0;

    if(argc != 3)
    {
        usage();
    }

    port = strtoul(argv[2], &dummy, 10);

    if(port < 1 || port > 65535 || *dummy != '\0')
    {
        fprintf(stderr, "Invalid port number: %s\n", argv[2]);
        usage();
    }

    error = getaddrinfo(argv[1], argv[2], &hints, &res0);
    if(error)
    {
        perror(gai_strerror(error));
        return (-1);
    }

    s= -1;
    for (res = res0; res; res = res->ai_next)
    {
        s = socket(res->ai_family, res->ai_socktype, 0);
        break;
    }

    sendto(s, send_data,8, 0,res->ai_addr, res->ai_addrlen);
    gettimeofday(&timestamp_send,NULL);

    recv_data_len = recvfrom(s,send_data,8,0,res->ai_addr, &res->ai_addrlen);

    while(recv_data_len==0){}

    gettimeofday(&timestamp_return,NULL);

    timersub(&timestamp_return, &timestamp_send, &tv_tmp);
    printf("Latency = %ld.%ld s\n",tv_tmp.tv_sec,tv_tmp.tv_usec);
}


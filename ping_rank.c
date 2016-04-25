#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* atoi */
#include <unistd.h> /* setuid, getuid, close */

#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h> /* getaddrinfo */
#include <sys/time.h>


int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}



int main(int argc, char * argv[])
{

    int size = 3;
    
    char tab_addr_in[39][size];
    char tab_addr_out[39][size];
    
    int tab_ping[size];
    float tab_sorted[size];
    float tab_unsorted[size];
    
    int tab_port[39][size];

    for(int f=0; f<size; f++)
    {

        struct addrinfo hints, *res, *res0;
    
        unsigned long port;
        char * dummy;
        const char *cause = NULL;
    
        int s, error, pck;
    
        float ping, ping_min, ping_max, ping_avg;
    
        struct timeval tv_tmp;
        struct timeval timestamp_send;
        struct timeval timestamp_return;
    
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
    
        int send_data[8];
        int recv_data_len=0;
        
        printf("IP or hostname %d : \n",i);
        scanf("%s",tab_addr_in[i]);
        
        printf("Port %d : \n",i);
        scanf("%s",tab_port[i]);

    
        port = strtoul(tab_port[i], &dummy, 10);
    
        if(port < 1 || port > 65535 || *dummy != '\0')
        {
            fprintf(stderr, "Invalid port number: %s\n", tab_port[i]);
            usage();
        }
    
        error = getaddrinfo(tab_addr_in[i], tab_port[i], &hints, &res0);
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
    
        pck = 3;
        ping = 0; ping_min = 0; ping_max = 0; ping_avg = 0;
    
        for(int i=0; i<pck; i++)
        {
            sendto(s, send_data,8, 0,res->ai_addr, res->ai_addrlen);
            gettimeofday(&timestamp_send,NULL);
    
            recv_data_len = recvfrom(s,send_data,8,0,res->ai_addr, &res->ai_addrlen);
    
            while(recv_data_len==0){}
            gettimeofday(&timestamp_return,NULL);
    
            timersub(&timestamp_return, &timestamp_send, &tv_tmp);
            ping = ((float)tv_tmp.tv_sec)*1000+((float)tv_tmp.tv_usec)/1000;
    
            if(ping<ping_min || ping_min==0)
            {
                ping_min = ping;
            }
            if(ping>ping_max)
            {
                ping_max = ping;
            }
            ping_avg = ping_avg+ping;
        }
        
        ping_avg=ping_avg/pck;
        tab_unsorted[f]=ping_avg;
        
    }
    
    qsort (tab_unsorted, size, sizeof(int), compare);
    
    for(int j=0; j<size; j++)
    {
        for(int k=0; k<size; k++)
        {
            if(tab_sorted[j]==tab_unsorted[k])
            {
                tab_addr_out[j] = tab_addr_in[k];
            }
        }
    }
}

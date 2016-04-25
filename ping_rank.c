#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* atoi */
#include <unistd.h> /* setuid, getuid, close */

#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h> /* getaddrinfo */
#include <sys/time.h>


int main(int argc, char * argv[])
{

    int size = 3;
    
    char tab_addr_in[size][39];
    char tab_addr_out[size][39];
    
    int tab_ping[size];
    float tab_sorted[size];
    float tab_unsorted[size];
    
    char tab_port[size][6];
    char tab_port_out[size][6];

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

if(f==1){ printf("Avant scanf : %s",tab_addr_in[0]);}

        printf("IP or hostname %d : \n",f);
        scanf("%s",tab_addr_in[f]);
if(f==1){ printf("Apres scanf : %s",tab_addr_in[0]);}
        printf("Port %d : \n",f);
        scanf("%s",tab_port[f]);

        port = strtoul(tab_port[f], &dummy, 10);
    
        if(port < 1 || port > 65535 || *dummy != '\0')
        {
            fprintf(stderr, "Invalid port number: %s\n", tab_port[f]);
        }
    
        error = getaddrinfo(tab_addr_in[f], tab_port[f], &hints, &res0);
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
printf("Apres socket : %s\n",tab_addr_in[f]);
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
        tab_sorted[f]=ping_avg;
        printf("Ping avg:%f\n",ping_avg);

        printf("IP :%s\n",tab_addr_in[0]);
        printf("Port :%s\n",tab_port[0]);

    }

    float value = 0.0f;
    for(int i=0;i<size-1;i++)
    {
        for(int j=0;j<size-i-1;j++)
        {
            if(tab_sorted[j]>tab_sorted[j+1])
            {
                value=tab_sorted[j+1];
                tab_sorted[j+1]=tab_sorted[j];
                tab_sorted[j]=value;
            }
        }
    }



    for(int n=0;n<size;n++){printf("Ping n°%d: %f\n",n,tab_sorted[n]);}

    for(int j=0; j<size; j++)
    {printf("J: %d\n",j);
        for(int k=0; k<size; k++)
        {printf("K: %d\n",k);
            if(tab_sorted[j]==tab_unsorted[k])
            {printf("OK\n");
               strcpy(tab_addr_out[j],tab_addr_in[k]);
printf("tab_addr_in : %s\n",tab_addr_in[k]);
               strcpy(tab_port_out[j],tab_port[k]);
printf("tab_port : %s\n",tab_port[k]);
            }
        }
    }
    for(int i=0; i<size; i++)
    {
        printf("Server n°%d : %s:%s\n",i,tab_addr_out[i],tab_port_out[i]);
    }
}

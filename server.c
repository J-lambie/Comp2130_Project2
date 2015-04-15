#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <arpa/inet.h>

#define BUF_SIZE	1024
#define LISTEN_PORT	60000
//client structure
struct clients{
	char username[10];
	int workgroup;//register status for workgroup
	int fungroup;//register status for workgroup
	//struct sockaddr_in ip_addr;	
};
//Registers client to a group based on input from client
//Returns 1 if sucessful and -1 if not sucessful
int Register(struct clients client,int group)
{
	if(group==1)
		client.workgroup=1;
	else
	if(group==2)
		client.fungroup=1;
	else
		return -1;
	return 1;
	
}
//Register Screen
char register_Screen[]="\nWhich group do you Wish to register for:\n1-WorkGroup\n2-FunGroup\n";



//main
int main(int argc, char *argv[]){
    int			sock_recv,bytes_sent;
    struct sockaddr_in	my_addr;
    int			i,j=0;
    fd_set	readfds,active_fd_set,read_fd_set;
    struct timeval		timeout={0,0};
    int			incoming_len;
    struct sockaddr_in	remote_addr;
    int			recv_msg_size;
    char			buf[BUF_SIZE];
    int			select_ret;
    struct clients client[200];

            /* create socket for receiving */
    sock_recv=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_recv < 0){
        printf("socket() failed\n");
        exit(0);
    }
        /* make local address structure */
    memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
    my_addr.sin_family = AF_INET;	/* address family */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */
    my_addr.sin_port = htons((unsigned short)LISTEN_PORT);
        /* bind socket to the local address */
    i=bind(sock_recv, (struct sockaddr *) &my_addr, sizeof (my_addr));
    if (i < 0){
        printf("bind() failed\n");
        exit(0);
    }
       FD_ZERO(&readfds);		/* zero out socket set */
       FD_SET(sock_recv,&readfds);	/* add socket to listen to */
        /* listen ... */
char onlineList[500]="Online\n-Workgroup\n-FunGroup\n";
char onlinePer[500]="";
    while (1){
        read_fd_set = active_fd_set;
        select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
	
        if (select_ret > 0){		/* anything arrive on any socket? */
	   
	   incoming_len=sizeof(remote_addr);	/* who sent to us? */
	
      	         
	   recv_msg_size=recvfrom(sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);
           /*puts("After receiving...");*/
            if (recv_msg_size > 0){	/* what was sent? */
                buf[recv_msg_size]='\0';
            	printf("From %s received: %s\n",inet_ntoa(remote_addr.sin_addr),buf);
        	strcpy(client[j].username,buf);
	    	sprintf(onlinePer,"%i - %s",j, client[j].username);
		strcat(onlinePer,"\n");
		strcat(onlineList, onlinePer);
		bytes_sent=sendto(sock_recv,onlineList,strlen(onlineList),0,(struct sockaddr *)&remote_addr,incoming_len); 
                 
             }
            }
        
        if (strcmp(buf,"shutdown") == 0)
            break;
	j++;
    }
    
    close(sock_recv);
}



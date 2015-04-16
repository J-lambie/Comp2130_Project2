//Group Members: 620067270 and 620069957
//due date:15 April, 2015
#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <arpa/inet.h>

#define BUF_SIZE	1024
#define LISTEN_PORT	60000
//Register Screen
char register_Screen[]="\nWhich group do you Wish to register for:\n1-WorkGroup\n2-FunGroup\n";
char register_Request[]="\nYou are not register for the group.\nWould you like to register(y/n)?\n";



//client structure
struct clients{
	char username[10];
	int workgroup;//register status for workgroup
	int fungroup;//register status for workgroup
	struct sockaddr_in addr;	
	int addr_len;
	int sock_recv;
};
struct clients client[200];
//Registers client to a group based on input from client
//Returns 1 if sucessful and -1 if not sucessful
int checkChat(int j,char* buf,int response)
{
	int bytes_sent,bytes_recv,i;
	buf[response]='\0';
	if(strncmp("*",buf,1)==0){
			//check if registered
			if(client[j].workgroup==1)
				//start group
				printf("work");
			else{
  				//sends if you want to register for group
				bytes_sent=sendto(client[j].sock_recv,register_Request,strlen(register_Request),0,(struct sockaddr *)&client[j].addr,client[j].addr_len);
				bytes_recv=recvfrom(client[j].sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&client[j].addr,&client[j].addr_len);
				buf[bytes_recv]='\0';
				if(strncmp(buf,"y",1)==0)
					client[j].workgroup=1;
				else
					client[j].workgroup=0;
			}
		}else{
		if(strncmp("+",buf,1)==0){
			//check if registered
			if(client[j].fungroup==1)
				//start group
				printf("fun");
			else{
  				//sends if you want to register for group
				bytes_sent=sendto(client[j].sock_recv,register_Request,strlen(register_Request),0,(struct sockaddr *)&client[j].addr,client[j].addr_len);
				bytes_recv=recvfrom(client[j].sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&client[j].addr,&client[j].addr_len);
				buf[bytes_recv]='\0';
				if(strncmp(buf,"y",1)==0)
					client[j].fungroup=1;
				else
					client[j].fungroup=0;
			}
		}else{
			for(i=0;i<response;i++){			
				if(isdigit(buf[i])==0)
					bytes_sent=sendto(client[j].sock_recv,"Incorrect Input",strlen("Incorrect Input"),0,(struct sockaddr *)&client[j].addr,client[j].addr_len);
				else{
					char chat_Request[200];
					strcpy(chat_Request,client[j].username);
					strcat(chat_Request," wishes to start a chat");
					bytes_sent=sendto(client[atoi(buf)].sock_recv,chat_Request,strlen(chat_Request),0,(struct sockaddr *)&client[atoi(buf)].addr,client[atoi(buf)].addr_len);
					
					//bytes_recv=recvfrom(client[atoi(buf)].sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&client[atoi(buf)].addr,&client[atoi(buf)].addr_len);
					//buf[bytes_recv]='\0';
					break;
					
				}
			}

		}
             }
		
}

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
    sock_recv=socket(AF_INET, SOCK_STREAM,0);
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
char onlineList[500]="Online\n* -Workgroup\n+ -FunGroup\n";
char onlinePer[500]="";
    while (1){
        read_fd_set = active_fd_set;
        select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
	
        if (select_ret > 0){		/* anything arrive on any socket? */
	   
	   incoming_len=sizeof(remote_addr);	/* who sent to us? */
	   client[j].addr=remote_addr;
	   client[j].addr_len=sizeof(remote_addr);
           client[j].sock_recv=sock_recv;
      	    //receives username     
	   recv_msg_size=recvfrom(sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);
           /*puts("After receiving...");*/
            if (recv_msg_size > 0){	/* what was sent? */
                buf[recv_msg_size]='\0';
            	printf("From %s received: %s\n",inet_ntoa(remote_addr.sin_addr),buf);
        	strcpy(client[j].username,buf);
	    	sprintf(onlinePer,"%i - %s",j, client[j].username);
		strcat(onlinePer,"\n");
		strcat(onlineList, onlinePer);
		strcat(onlineList,"Who do you want to start a chat with");
		//sends list of people online
		bytes_sent=sendto(sock_recv,onlineList,strlen(onlineList),0,(struct sockaddr *)&remote_addr,incoming_len); 
		//receives requests for convo.
                recv_msg_size=recvfrom(sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);
		buf[recv_msg_size]='\0';
		//checkChat(j,buf,recv_msg_size);
		
            }
        }
        if (strcmp(buf,"shutdown") == 0)
            break;
	j++;
    }
    
    close(sock_recv);
}



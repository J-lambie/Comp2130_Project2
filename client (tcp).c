//Group Members: 620067270 and 620069957
//due date:15 April, 2015
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */


#define BUF_SIZE	1024
#define SERVER_IP	"127.0.0.1"
#define	PORT		60000

int main(int argc, char *argv[]){
    int			sock_send;
    struct sockaddr_in	addr_send;
    char			text[80],buf[BUF_SIZE],username[20];
    int			send_len,bytes_sent,bytes_received,incom_len;
    /* create socket for sending data */
    sock_send=socket(AF_INET, SOCK_STREAM, 0);
    if (sock_send < 0){
        printf("socket() failed\n");
        exit(0);
    }

            /* fill the address structure for sending data */
    memset(&addr_send, 0, sizeof(addr_send));  /* zero out structure */
   
    addr_send.sin_family = AF_INET;  /* address family */
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_send.sin_port = htons(PORT);
 if (connect(sock_send,(struct sockaddr *) &addr_send,sizeof(addr_send)) < 0) {
        printf("ERROR connecting");
        exit(0);
    }
    //Welcome Prompt
    char welcomeprompt[]="\n Welcome to the chat\nEnter a username to use: ";
    printf("%s",welcomeprompt);
    scanf("%s",username);
    //sends username
    bytes_sent=sendto(sock_send, username, sizeof(username), 0,(struct sockaddr *) &addr_send, sizeof(addr_send));
    while(1){
	//receives online list
	bytes_received=recvfrom(sock_send,text,200,0,(struct sockaddr *)&addr_send, &incom_len);
	text[bytes_received]='\0';
	printf("%s",text);
	scanf("%s",text);
	if (strcmp(text,"quit") == 0)
            break;

        strcpy(buf,text);
        send_len=strlen(text);
	incom_len =  sizeof(addr_send);
	//sends input to server
        bytes_sent=sendto(sock_send, buf, send_len, 0,(struct sockaddr *) &addr_send, sizeof(addr_send));
	//listen to response
        //bytes_received=recvfrom(sock_send,text,200,0,(struct sockaddr *)&addr_send, &incom_len);
	//text[bytes_received]='\0';
	//printf("%s",text);
	  }
    
    close(sock_send);
}




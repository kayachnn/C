#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <errno.h>
#define MAX 2048 



//function to chat between server and client
void func(int sockfd, char user_pass[],int len) 
{   
    
    char buff[MAX];
    bzero(buff, sizeof(buff));
    int n;

    //first send username and password to server
    
    printf("authetication user_pass:%s\n",user_pass);
    write(sockfd,user_pass,sizeof(char)*len);
    printf("user_pass has been sent\n");
    memset(user_pass,0,len);
    //read the message 
    read(sockfd, buff, sizeof(buff));
    printf("message from server: %s\n",buff);
    //memset(buff,0,sizeof(buff));
    
    if ((strncmp(buff,"true",4)) == 0)
    {
            //if credentials are true
        printf("credentials are true\n");
        for (;;)
        { 
            bzero(buff, sizeof(buff)); 
            printf("$ "); 
            n = 0; 
            while ((buff[n++] = getchar()) != '\n');
            write(sockfd, buff, sizeof(buff));

            bzero(buff, sizeof(buff));
         
            read(sockfd, buff, sizeof(buff)); 
            printf("\n%s\n",buff); 
        

            if ( ((strncmp(buff, "exit", 4)) == 0) || (strcmp(buff, "")) == 0)  { 
            printf("Client Exit...\n"); 
            break; 
            }
        }

    }

} 








//function to conver hostname to ip
void hostname_to_ip(char* , char *);
  
int main(int argc, char* argv[]) 
{
//first get the parameters ofthe program -h host -P port -u user -p pass
    
    int option;
    char* host = malloc(sizeof(char)*100);
    char* port = malloc(sizeof(char) * 100);
    char*  user_name = malloc(sizeof(char)*50);
    char* password = malloc(sizeof(char) * 50); 
    char* user_pass = malloc(sizeof(char)* 100); //authetication purpose

    while (( option = getopt(argc, argv, "hPup")) != -1)
    {
        switch (option)
        {
            case 'h':
                host = argv[2];
                printf("host is %s\n",host);
                break;
            case 'P':
                port = argv[4];
                printf("port is %s\n", port);
                break;
            case 'u':
                user_name = argv[6];
                printf("user is %s\n",user_name);
                break;
            case 'p':
                password = argv[8];
                printf("password is %s\n",password);
                break;
            case '?':
                printf("Usage: ./client -h hostname -P port -u user -p password\n");
                exit(-1);
        }
    }
//port is string so convert it to int
    int PORT = atoi(port);
    char ip[100];
    strcat(user_pass, user_name);
    strcat(user_pass, password);
    printf("user_pass = %s\n",user_pass);
    
    hostname_to_ip(host,ip);
    printf("ip = %s\n",ip);


//code for the socket part
    int client_socket; 
    struct sockaddr_in servaddr; 
  
    // socket create and varification 
    client_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (client_socket == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(ip); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(client_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    func(client_socket,user_pass,100); 
    
    // close the socket 
    close(client_socket); 
}



//Get ip from the domain name
void hostname_to_ip(char* hostname, char* ip)
{
    struct hostent *hostinfo;
    struct in_addr **addr_list;
    int i;

    if ( ( hostinfo = gethostbyname(hostname)) == NULL)
    {
        // get host info
        printf("could not get host info");
        exit(-1);
    }

    addr_list = (struct in_addr **)hostinfo->h_addr_list;

    for(i=0; addr_list[i] != NULL; i++)
    {
        strcpy(ip, inet_ntoa(*addr_list[i]) );
    }
}
     







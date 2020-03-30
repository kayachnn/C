#include <stdio.h>
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <getopt.h>
#include <signal.h>
#define MAX 2048  



//function for signal handling
void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
     printf("Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
     {
          exit(0);
     }
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}








// Function designed for chat between client and server. 
void func(int sockfd, char user_pass[], int len) 
{ 
    
     	//first read the user and password that has been sent by client
    	char message_true[] = "true";
        char message_false[] = "false credentials\n";
        char message_exit[] = "exit";
        char client_credentials[100];
        char* redirect_stderr = malloc(sizeof(char)* 5);
        redirect_stderr = " 2>&1";
        memset(client_credentials, '\0', sizeof(client_credentials));

        //read the message and copy it into client_credentials
        read(sockfd, client_credentials, sizeof(client_credentials));
        printf("client credentials are : %s\n",client_credentials);
    
       	//check if client credentials that were sent are correct
        printf("user and password in the system: %s\n",user_pass);
        if (strcmp(user_pass, client_credentials) == 0)
        {
            printf("credentials are true , user logged in\n");
            write(sockfd, message_true, sizeof(message_true));       
 
            // infinite loop for chat 

            for (;;)
            { 
	            char buff[MAX];
                int n; 
                memset(buff, '\0', sizeof(buff));
                // read the message from client and copy it in buffer 
                read(sockfd, buff, sizeof(buff)); 
                // print buffer which contains the client contents 
                buff[strlen(buff)-1] = '\0';


                
                if( strncmp(buff, "exit",4) == 0)
                {
                    printf("Closing connection...\n ");
                    write(sockfd, message_exit, sizeof(message_exit));
                    //close(sockfd);
                    exit(-1);
                
                }
                else if(strcmp(buff,"") == 0)
                {


                    printf("Closing connection...\n ");
                    write(sockfd, message_exit, sizeof(message_exit));
                    //close(sockfd);
                    exit(-1);


                }
                else
                {

// first problem with section below is that when is gets the output
// of the command it sends errors to stderr
// but we need errors too , to send it to client
// so we need to also get errors too
// so we will redirect stderr to stdout
// we need to use 2>&1 with every command
// strcat(command, " 2>&1")
// command from client is  stored in buff
// so code will be like strcat(buff, " 2>&1")
//

                    //section to execute command and read output
                    printf("shell command you typed: %s\n", buff);
                    strcat(buff,redirect_stderr);
                    FILE *fd;
                    fd = popen(buff,"r");
                    char buffer[MAX];
                    size_t chread;  //string to store entire command contents
                    size_t comalloc = MAX;
                    size_t comlen = 0;
                    char *comout = malloc(comalloc);

                    //use fread so binary data is dealt with correctly
                    while((chread = fread(buffer, 1, sizeof(buffer), fd)) != 0)
                    {
                        if (comlen + chread >= comalloc)
                            {
                            comalloc *=2;
                            comout = realloc(comout, comalloc);
                        }
                        memmove(comout + comlen, buffer, chread);
                        comlen += chread;
                    }
                    strcpy(buff,comout);
                    //fwrite(comout, 1, comlen,stdout);
                    write(sockfd, comout, comlen);
                    //write(sockfd, buff, sizeof(buff));
                    free(comout);
                    pclose(fd);
                    printf("\n");

                    // and send that buffer to client 
                    //write(sockfd, buff, sizeof(buff));
            
                    memset(comout, '\0', sizeof(comout));

                }
 


            }

        }

        else
        {
            printf("false credentials\n");
            write(sockfd, message_false, sizeof(message_false));
            // write(sockfd, message_exit, sizeof(message_exit));
            close(sockfd);
        }


     
} 
  


int main(int argc, char *argv[]) 
{ 
//first get the parameters ofthe program -P port -u user -p pass
     signal(SIGINT, INThandler);

         // pause();


    int option;
    char* port = malloc(sizeof(char)*100);
    char *user_name = malloc(sizeof(char)*50);
    char *password = malloc(sizeof(char)*50);
    char * user_password = malloc(sizeof(char)*100);

    while (( option = getopt(argc, argv, "Pup")) != -1)
    {
        switch (option)
        {
            case 'P':
                    port = argv[2];
                    printf("port is %s\n",port);
                    break;
            case 'u':
                    user_name = argv[4];
                    printf("The user is %s\n",user_name);
                    break;
            case 'p':
                    password = argv[6];
                    printf("password is %s\n",password);
                    break;
            case '?':
                    printf("Usage: ./server -P port -u username -p password\n"    );
                    exit(-1);
     
      } //end block for switch
    }  //end block for while

// port is string so convert it to int
    int PORT = atoi(port);
    strcat(user_password, user_name);
    strcat(user_password, password);




// code for the socket part
    int server_socket, connection_socket, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    server_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_socket == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(server_socket, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(server_socket, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connection_socket = accept(server_socket, (struct sockaddr*)&cli, &len); 
    if (connection_socket < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    // Function for chatting between client and server 
    func(connection_socket, user_password, 100); 
  
    // After chatting close the socket 
    close(server_socket); 

    
}

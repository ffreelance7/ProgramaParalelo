#include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <string>
  #include <iostream>
  #include <thread> 
  using namespace std;
int create_client(); 

char buffer_recive[256];
char buffer_send[256]; 
string s_buffer;
void _read(int SocketFD){
    do{
        bzero(buffer_recive,256);
        read(SocketFD,buffer_recive,256);   
        buffer_recive[sizeof(buffer_recive)]='\0';
        printf("Msn from Server>> : [%s]\n",buffer_recive);    

    }while(buffer_recive!="chau");
  }
void _write(int SocketFD){
    do{
	getline(cin,s_buffer);
	strcpy(buffer_send,s_buffer.c_str());
        write(SocketFD,buffer_send,strlen(buffer_send));
    }while(buffer_send!="chau");

  }  
int create_client(){
	struct sockaddr_in stSockAddr;
	int Res;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int n;
	
	if (-1 == SocketFD)
	{
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
	
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(5435);
	Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
	
	if (0 > Res)
	{
		perror("error: first parameter is not a valid address family");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
	else if (0 == Res)
	{
		perror("char string (second parameter does not contain valid ipaddress");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
	
	if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
	{
		perror("connect failed");
		close(SocketFD);	
		exit(EXIT_FAILURE);
	}
	return SocketFD;
}

int main(void){
    int SocketFD = create_client();  

    thread t_write (_write,SocketFD);    
    thread t_read (_read,SocketFD); 
   
    t_read.join();  
    t_write.join(); 

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
    return 0;
}


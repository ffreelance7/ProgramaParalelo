  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <cstdlib>
  #include <string>
  #include <iostream>
  #include <thread> 
  #include <map>
  #include <vector>
  using namespace std;
  
int create_socket(); 
void _write(int ,string);
char Matrix[3][3]={'-','-','-','-','-','-','-','-','-'};
char Symbol[2]={'X','0'};
int cont=0;
bool winner=false;
map<int,char> Ficha;
int create_socket(){
	struct sockaddr_in stSockAddr;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int n;

	if(-1 == SocketFD)
	{
		perror("can not create socket");
		exit(EXIT_FAILURE);
	}

	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(5435);
	stSockAddr.sin_addr.s_addr = INADDR_ANY;

	if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
	{
		perror("error bind failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	if(-1 == listen(SocketFD, 10))
	{
		perror("error listen failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	return SocketFD;
}

void Dibujar(){ 
 while(!winner){
   system("clear");
   for(int i=0;i<3;i++){ 
     for(int j=0;j<3;j++){ 
	cout<<Matrix[i][j];
     }
     cout<<endl;
  } 
  sleep(1);
 }
}
void validar(int ConnectFD){
 char val=Ficha.find(ConnectFD)->second; 
    if(Matrix[0][0]==Matrix[1][0]&&Matrix[1][0]==Matrix[2][0]&&Matrix[2][0]==val||
       Matrix[0][0]==Matrix[1][1]&&Matrix[1][1]==Matrix[2][2]&&Matrix[2][2]==val||
       Matrix[0][0]==Matrix[0][1]&&Matrix[0][1]==Matrix[0][2]&&Matrix[0][2]==val||
       Matrix[1][0]==Matrix[1][1]&&Matrix[1][1]==Matrix[1][2]&&Matrix[1][2]==val||
       Matrix[2][0]==Matrix[1][1]&&Matrix[1][1]==Matrix[0][2]&&Matrix[0][2]==val||
       Matrix[2][0]==Matrix[2][1]&&Matrix[2][1]==Matrix[2][2]&&Matrix[2][2]==val||
       Matrix[0][1]==Matrix[1][1]&&Matrix[1][1]==Matrix[2][1]&&Matrix[2][1]==val||
       Matrix[0][2]==Matrix[1][2]&&Matrix[1][2]==Matrix[2][2]&&Matrix[2][2]==val){
       	_write(ConnectFD,"GANASTE!!");
  }
}
bool Colocar(int ConnectFD){  
  char buffer[256];
  bzero(buffer,256);
  int posx=0;
  int posy=0;
  read(ConnectFD,buffer,1);
  posx = atoi(buffer);
  read(ConnectFD,buffer,1);
  posy = atoi(buffer);
  Matrix[posx][posy] = Ficha.find(ConnectFD)->second; 
  validar(ConnectFD);
}
void _read(int ConnectFD){
   char buffer[256];
   while(true){        
        bzero(buffer,256);     
        read(ConnectFD,buffer,1); 
        switch(buffer[0]){
                case '1': Colocar(ConnectFD);
			break;
		default: cout << "OPCION NO VALIDA" << endl;
         }               
   }
}
void _write(int ConnectFD,string msn){
     char buffer[256];
     strcpy(buffer,msn.c_str());
     write(ConnectFD,buffer,sizeof(buffer));
}
int main(void){
  int SocketFD = create_socket();
  thread(Dibujar).detach(); 
  for(;;){ 
	int ConnectFD = accept(SocketFD, NULL, NULL);
        if(ConnectFD<0) return 0;
        Ficha.insert (pair<int,char> (ConnectFD,Symbol[cont]));
        cont++; 
        thread(_read,ConnectFD).detach(); 	
  }

  close(SocketFD);
  return 0;
}


///////////LEA-128 ECB MODE 16BYTE DATA////////////////////
#ifdef RaspberryPi 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdint.h> //uint8_t definitions
#include <stdlib.h> //for exit(int);
#include <string.h> //for errno
#include <errno.h> //error output
#include <wiringPi.h>
#include <wiringSerial.h>
#include "LEA_core.h"
#include "LEA_core.c"

char device[]= "/dev/ttyACM1";
int fd;
unsigned long baud = 9600;
unsigned long time=0;
 
BYTE pbUserKey[16] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0 };
BYTE pdwRoundKey[384] = { 0x0, };
//BYTE pbData[16] = {  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
 
void setup()
{
 
  printf("%s \n", "Raspberry Startup!");
  fflush(stdout);
 
  //get filedescriptor
  if ((fd = serialOpen (device, baud)) < 0){
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    exit(1); //error
  }
 
  //setup GPIO in wiringPi mode
  if (wiringPiSetup () == -1){
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    exit(1); //error
  }
 
}

void loop(char *data){
	
  // Pong every 3 seconds
  if(millis()-time>=3000){
    serialPuts (fd, "Pong!\n");
    // you can also write data from 0-255
    // 65 is in ASCII 'A'
    serialPutchar (fd, 65);
    time=millis();
  }
 
  // read signal
  if(serialDataAvail (fd)){
    //data[0] = serialGetchar(fd);
    
    read(fd, data, 1);
    //data[0] = serialGetchar(fd);
    //printf("char : %c\n",data[0]);
    fflush(stdout);
  }
 
}
 
int main(int argc, char *argv[])
{
int fd_socket;
struct sockaddr_in addr;
struct hostent *host;
char buf[100];
char buffer[BUFSIZ];
int i, j = 0;
char data[16] ={ 0x00,};
setup();

if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)

{

perror("client: socket");
return 1;
}


//-DNS-

host = gethostbyname(argv[1]);
if( host == NULL)
{

printf("Host not found !!: %s\n\r",argv[1]);
return;

}

bzero((char *)&addr, sizeof(addr));
addr.sin_family =AF_INET;
memcpy((char *)&addr.sin_addr, host->h_addr, host->h_length);
addr.sin_port = htons(atoi(argv[2]));

inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
printf("Trying %s ...\n",buf);
if(connect(fd_socket, (struct sockaddr *)&addr, sizeof(addr)) ==-1)
{
perror("client: connect");
return 1;
}

printf("connected %s...\n",argv[1]);



while(1) 
  {
	loop(&data[i]);
   i++;
   sleep(0.5);
   if(data[i-1] == 0x0a)
   {
         
      LEA_Key(pbUserKey, pdwRoundKey);
      LEA_Enc(pdwRoundKey,data);
      printf("Encrypt: ");
      for(j = 0; j < 16; j++)
      {
      printf("%02x ", data[j]);
      }
      printf("\n");
      write(fd_socket, data, 16);
      
      memset(data, 0x00, 16);
      i = 0;
      sleep(1);
   }
      
	  
  }
close(fd_socket);
return 0;
}
#endif 


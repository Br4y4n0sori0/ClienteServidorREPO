#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
void gotoxy(int x,int y)
 {
 printf("%c[%d;%df",0x1B,y,x);
 }
int main(int argc, char *argv[])   // declaracion e iniciacion de variables.
{
    system("Despejado");
    int sockfd = 0;  // descriptor de archivo, en C los descriptores van indicados en enteros
    int bytesReceived = 0; // numero de bytes recibidos
    char recvBuff[1024];   //buffer para recibir los datos
    memset(recvBuff, '0', sizeof(recvBuff)); //Inicializacion del buffer en ceros, llenar area de memoria en ceros
    struct sockaddr_in serv_addr; // sockaddr_in es el nombre de la estructura, serv_addr es el nombre de la variable

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)   // inicializacion del socket
    {
        printf("\n No se pudo crear el socket \n");
        return 1;
    }

    /* Inicializar socket en estructura de datos */
    serv_addr.sin_family = AF_INET;    //familia de direcciones -- AF_INET= IPV4
    serv_addr.sin_port = htons(5000); // puerto
    char ip[50];     // String que contiene los caracteres de las direcciones
if (argc < 2) // Lee por consola la direccion ip
{
    printf("Ingrese la direccion IP del cliente: ");
    gets(ip);  // gets=lee caracteres por consola(direccion ip)
}
else   //toma la direccion IP insertada por consola
  strcpy(ip,argv[1]);  

    serv_addr.sin_addr.s_addr = inet_addr(ip);  // toma el char ip y lo convierte a formato direccion

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Conexion fallida \n");
        return 1;
    }

    printf("Conectado a IP: %s : %d\n",inet_ntoa(serv_addr.sin_addr),ntohs(serv_addr.sin_port)); //imprime la direccion IP ; puerto

   	 /* Create file where data will be stored */
    	FILE *fp; //descriptor de archivo 
	char fname[100];  //nombre del archivo
	read(sockfd, fname, 256); //esta leyendo desde el socket, un nombre de archivo - read(fileDescriptor,VariableDeDondeSeLeeranDatos,Tamaño maximo del buffer)
	//strcat(fname,"AK");
	printf("Nombre de archivo: %s\n",fname);
	printf("Recibiendo archivo...");
   	 fp = fopen(fname, "ab"); //fopen= abrir archivo en el descriptor designado fp / "ab" indica el modo de apertura del archivo
    	if(NULL == fp)
    	{
       	 printf("Error al abrir archivo");
         return 1;
    	}
    long double sz=1;
    /* Receive data in chunks of 1024 bytes */
    while((bytesReceived = read(sockfd, recvBuff, 1024)) > 0)
    { 
        sz++;
        gotoxy(0,4);
        printf("Recibido: %llf Kb",(sz/1024));  // Impresion dejando 11 espacios en punto flotante
	fflush(stdout);  // Vaciar los bufferes o descriptores de archivos en pantalla
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp);  // Escribir datos del buffer(recvBuff), de un byte por longitud, numero de bytes a escribir(bytesReceived), la variable destino(fp)(descriptor de archivo)
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
        printf("\n Error al leer \n");
    }
    printf("\nArchivo recibido....Completado\n");
    return 0;
}



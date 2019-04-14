#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


struct sockaddr_in c_addr;
char fname[100];

void* EnviarArchivoCliente(int *arg)
{
      int connfd=(int)*arg;   // numero del socket asignado por el sistema operativo
      printf("Conexion aceptada con id: %d\n",connfd);
      printf("Conectado a cliente: %s:%d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
       write(connfd, fname,256);

        FILE *fp = fopen(fname,"rb");   // lectura del archivo, "r"= read, "b"= binary
        if(fp==NULL)
        {
            printf("Error al abrir el archivo");
            return 1;   
        }   

        /* Leer datos desde el archivo y enviarlos */
        while(1)
        {
            
            unsigned char buff[1024]={0};  // Inicializacion del buffer
            int nread = fread(buff,1,1024,fp);
                   

            /* Si se lee exitosamente, se envian los datos */
            if(nread > 0)
            {
                
                write(connfd, buff, nread);
            }
            if (nread < 1024)
            {
                if (feof(fp))  // condicional para verificar que se termino de leer el archivo
		{
                    printf("Fin del archivo\n");
		    printf("Transferencia de archivo completada para id: %d\n",connfd);  //connfd= socket por el que se conecto al cliente
		}
                if (ferror(fp))  //
                    printf("Error en lectura\n");
                break;
            }
        }
printf("Cerrando conexion para id: %d\n",connfd);
close(connfd);
shutdown(connfd,SHUT_WR); // se esta cerrando la conexion al socket , SHUT_WR cierra la conexion para lectura y escritura
sleep(2);
}

int main(int argc, char *argv[])  //argc viene inicializado en 0
{
    int connfd = 0,err;  //inicializan el descriptor de socket en 0
    pthread_t tid; 
    struct sockaddr_in serv_addr;
    int listenfd = 0,ret;  // listenfd= listener del socket
    char sendBuff[1025];  // buffer de transmision
    int numrv;
    size_t clen=0;  // size_t se usa para describir el tamaño de algun tipo de dato

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0)
	{
	  printf("Error en creacion de socket\n");
	  exit(2);
	}

    printf("Obtencion de socket exitosa\n");   // Asignacion de familia de direcciones

    serv_addr.sin_family = AF_INET; //AF_INET= familia de direcciones
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY = aceptar conexiones desde cualquier ip, htonl = convertir de formato host a formato de red
    serv_addr.sin_port = htons(5000);

    ret=bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));  //bind= asignar la ip designada al socket
    if(ret<0)
    {
      printf("Error en bind\n");
      exit(2);
    }

    if(listen(listenfd, 10) == -1)
    {
        printf("Fallo al escuchar\n");
        return -1;
    }

if (argc < 2)   // 
{
	printf("Ingresar nombre del archivo a enviar: ");
        gets(fname); // gets= leer por consola
}
else
   strcpy(fname,argv[1]); // strcpy(destino,origen) , toma el nombre del archivo desde el argumento y lo asigna a la variable(en caso de que no se le pase el argumento por consola)

    while(1)
    {
        clen=sizeof(c_addr);  // clen se usa para saber el tamaño de la estructura(direccion del cliente)
        printf("En espera...\n");
        connfd = accept(listenfd, (struct sockaddr*)&c_addr,&clen); // espera la conexion de un cliente, se detiene hasta que se conecte un cliente
        if(connfd<0)
        {
	  printf("Error al aceptar conexion\n");
	  continue;	
	}
        err = pthread_create(&tid, NULL, &EnviarArchivoCliente, &connfd); //se crea el hilo para enviar el archivo
        if (err != 0)
            printf("\nError al crear hilo :[%s]", strerror(err));
   }
    close(connfd);
    return 0;
}

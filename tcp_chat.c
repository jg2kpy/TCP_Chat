//Standart Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//API to conection TCP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Defines
#define port 6969
#define Protocol_IP AF_INET
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

int main(int argc, char *argv[]){
    if (argc != 2){
        perror("USE: ");
        return 0;
    }

    int network_socket = socket(Protocol_IP, TCP, 0); //SOCKET FOR TCP/IP

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, "192.168.0.10", &server_address.sin_addr);

    if (strcmp(argv[1], "-client") == 0){

        int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

        if (connection_status == -1){
            perror("ERROR: connect()");
        }

        //recieve data from the server
        char server_reponse[256];
        recv(network_socket, &server_reponse, sizeof(server_reponse), 0);

        //print the server message
        //printf("Server: %s", server_reponse);

        close(network_socket);
    }else if (strcmp(argv[1], "-server") == 0){

        //server_address.sin_addr.s_addr = INADDR_ANY;

        //create the socket

        //bind the socket to our specified IP and port
        bind(network_socket, (struct sockaddr *)&server_address, sizeof(server_address)); //Igual que connect()

        //listening for connection
        listen(network_socket, 1); //1 Parametro: ID del socket. 2 Parametro: cantidad de conexiones

        int client_socket = accept(network_socket, NULL, NULL); //INVESTIGAR MAS ESTO

        char buffer[256] = "Hello";
        send(client_socket, buffer, sizeof(buffer), 0); //Enviar mensaje desde el server al cliente

        //close the socket
        close(network_socket);
    }else{
        perror("ERROR: Especifica el modo");
        exit(EXIT_FAILURE);
    }
    printf("Mensaje enviado, cerrando el server");
    return 0;
}
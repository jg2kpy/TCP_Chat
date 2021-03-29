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
#define protocolIP AF_INET
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

int network_socket = -1;
struct sockaddr_in server_address;

struct contact{
    char* name;
    char* IP;
}typedef contact;

contact Receptor;


void printError(char *errorMessage){
    fprintf(stderr,"Error message: %s\n",errorMessage);
    exit(EXIT_FAILURE);
}

void usage(){
    printf("Usage");
}

void setSocketAndAddress(){
    network_socket = socket(protocolIP, TCP, 0); //SOCKET FOR TCP/IP

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    Receptor.IP = "192.168.0.10";
    inet_pton(AF_INET, Receptor.IP, &server_address.sin_addr);
}

void waitMode(){
    int counter;
    int connection_status = -1;
    while (connection_status == -1){
        int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
        counter++;
        if(counter==30){
            printError("Cant connect after 30 trys");
        }
    }
    //recieve data from the server
    char server_reponse[256];
    recv(network_socket, &server_reponse, sizeof(server_reponse), 0);

    //print the server message
    printf("Server: %s", server_reponse);

    close(network_socket);
}

void startMode(){
    //create the socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)); //Igual que connect()

    //listening for connection
    listen(server_socket, 5); //1 Parametro: ID del socket. 2 Parametro: cantidad de conexiones

    int client_socket = accept(server_socket, NULL, NULL); //INVESTIGAR MAS ESTO

    char server_message[256] = "HOLA";
    send(client_socket, server_message, sizeof(server_message), 0); //Enviar mensaje desde el server al cliente

    //close the socket
    close(server_socket);
}


int main(int argc, char *argv[]){
    if (argc != 3){
        usage();
    }
    //searchContact();
    setSocketAndAddress();
    if (strcmp(argv[1], "-wait") == 0 || strcmp(argv[1], "-w") == 0){
        waitMode();
    }else if (strcmp(argv[1], "-start") == 0 || strcmp(argv[1], "-s") == 0){
        startMode();
    }else{
        usage();
    }
    return 0;
}
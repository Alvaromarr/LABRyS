#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> 

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    
    if (argc < 3) {
        printf("Error: Faltan argumentos.\n");
        printf("Argumentos <puerto> \"<mensaje>\"\n");
        return 1;
    }

    
    int puerto = atoi(argv[1]); 
    char *mensaje = argv[2];    

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return 1;
    }

    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(puerto); 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    
    sendto(sockfd, mensaje, strlen(mensaje), 0, 
           (const struct sockaddr *)&server_addr, addr_len);
    
    
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                     (struct sockaddr *)&server_addr, &addr_len);
    
    if (n > 0) {
        buffer[n] = '\0';
        printf("Respuesta del servidor: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
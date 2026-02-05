#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> 

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    
    if (argc < 2) {
        printf("Especifica el puerto como argumento.\n");
        return 1;
    }

    int puerto = atoi(argv[1]); 

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto); 

   
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        return 1;
    }

    printf("Servidor UDP escuchando en el puerto %d...\n", puerto);

    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                         (struct sockaddr *)&client_addr, &addr_len);
        
        buffer[n] = '\0';
        printf("Cliente [%s:%d] dice: %s\n", 
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);


        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> 

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if (argc < 2) {
        printf("Especifica el puerto como argumento.\n");
        return 1;
    }

    int puerto = atoi(argv[1]); 

    // Cambio a SOCK_STREAM para TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto); 

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        return 1;
    }

    listen(sockfd, 5);

    printf("Servidor TCP escuchando en el puerto %d...\n", puerto);

    while (1) {
    
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        
        int n = recv(newsockfd, buffer, BUFFER_SIZE, 0);
        
        if (n > 0) {
            buffer[n] = '\0';
            printf("Cliente [%s:%d] dice: %s\n", 
                    inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

            send(newsockfd, buffer, n, 0);
        }

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
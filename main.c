#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// o html é no formato latino, por isso é necesário usar o charset=iso-8859-1
const char *html_content = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n\r\n"
"<!DOCTYPE html charset=iso-8859-1\>"
"<html>"
"<head><title>Servidor Carlos</title></head>"
"<body>"
"<h1>Controlador de Acesso</h1>"
"<p>Olá, seja bem-vindo ao controlador de acesso de Carlos!</p>"
"<p>Este é um servidor que foi criado para demonstrar o funcionamento de um servidor web.</p>"
"<p>Este servidor foi criado por Carlos Henrique.</p>"
"<p>Este servidor foi criado para a disciplina de Redes de Computadores.</p>"
"<p>Este servidor foi criado para a Universidade Federal de Itajuba.</p>"
"<body>"
"</html>";

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read failed");
        close(client_socket);
        return;
    }
    ssize_t bytes_written = write(client_socket, html_content, strlen(html_content));
    if (bytes_written < 0) {
        perror("write failed");
    }
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        handle_client(client_socket);
    }

    return 0;
}
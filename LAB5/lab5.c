#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
void handle_client(int client_socket, struct sockaddr_in client_addr) {
    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // In the child process, redirect standard output to the client socket
    dup2(client_socket, STDOUT_FILENO);
    close(client_socket);

    // Execute the "sl -l" command
    execlp("sl", "sl", "-l", (char *)NULL);

    // If execlp fails, print an error message
    perror("Exec failed");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int port = atoi(argv[1]);  // Convert the port argument to an integer

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the server address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Acceptance failed");
            continue;
        }

        // Use fork() to create a child process
        pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {  // Child process handles the client
            close(server_socket);  // Close unnecessary socket
            handle_client(client_socket, client_addr);
            exit(EXIT_SUCCESS);
        } else {  // Parent process continues waiting for the next connection
            close(client_socket);
            // Parent process doesn't wait for the child process to avoid zombie processes
            int a = waitpid(-1, NULL, WNOHANG);
            printf("child process end or not: %d", a);
            
        }
    }

    // Close the main server socket  
    close(server_socket);
    return 0;
}


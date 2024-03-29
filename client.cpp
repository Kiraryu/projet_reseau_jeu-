#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void displayBoard(char board[3][3]) {
    std::cout << "Board:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};
    char board[3][3];

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server" << std::endl;

    while (true) {
        // Receive current board from server
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        memcpy(board, buffer, sizeof(board));

        // Display board
        displayBoard(board);

        // Get player's move
        int row, col;
        std::cout << "Enter row and column (0-2): ";
        std::cin >> row >> col;
        sprintf(buffer, "%d %d", row, col);

        // Send move to server
        send(clientSocket, buffer, strlen(buffer), 0);

        // Receive updated board from server
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        memcpy(board, buffer, sizeof(board));
    }

    close(clientSocket);
    return 0;
}


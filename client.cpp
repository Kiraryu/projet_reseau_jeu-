#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char board[3][3];
    int choice;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        return -1;
    }

    // Play the game
    while (/* condition for game continuation */) {
        // Receive the current state of the board
        recv(clientSocket, board, sizeof(board), 0);

        // Display the board to the player
        // (Implement display logic here)

        // Get player's choice
        std::cout << "Enter your move (1-9): ";
        std::cin >> choice;

        // Send choice to the server
        send(clientSocket, &choice, sizeof(choice), 0);

        // Receive the updated board from the server
        recv(clientSocket, board, sizeof(board), 0);
    }

    close(clientSocket);
    return 0;
}

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

void playGame(int clientSocket) {
    char board[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char message[100];
    int choice;
    int currentPlayer = 0;

    do {
        // Send the current state of the board to clients
        send(clientSocket, board, sizeof(board), 0);

        // Receive the choice from the current player
        recv(clientSocket, &choice, sizeof(choice), 0);

        // Update the board with the current player's move
        char symbol = (currentPlayer == 0) ? 'X' : 'O';
        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = symbol;

            // Check for win condition
            if (checkWin(board)) {
                // Inform clients about win
                sprintf(message, "Player %c wins!\n", symbol);
                send(clientSocket, message, sizeof(message), 0);
                break;
            }

            // Check for tie
            if (checkTie(board)) {
                // Inform clients about tie
                strcpy(message, "It's a tie!\n");
                send(clientSocket, message, sizeof(message), 0);
                break;
            }

            currentPlayer = 1 - currentPlayer; // Switch player
        }
    } while (/* condition for game continuation */);

    // Send the final state of the board to clients
    send(clientSocket, board, sizeof(board), 0);
}


bool checkWin(char board[3][3]) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return true;
        }
    }

    // Check diagonals
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
        return true;
    }

    return false;
}

bool checkTie(char board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false;
            }
        }
    }
    return true;
}


int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Binding failed\n";
        return -1;
    }

    // Listen for clients
    if (listen(serverSocket, 2) < 0) {
        std::cerr << "Listening failed\n";
        return -1;
    }

    std::cout << "Waiting for players...\n";

    while (true) {
        // Accept connection from client
        newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
        if (newSocket < 0) {
            std::cerr << "Acceptance failed\n";
            return -1;
        }

        std::cout << "Player connected\n";

        // Create a child process to handle the game
        if (fork() == 0) {
            close(serverSocket);
            playGame(newSocket);
            close(newSocket);
            return 0;
        } else {
            close(newSocket);
        }
    }

    close(serverSocket);
    return 0;
}




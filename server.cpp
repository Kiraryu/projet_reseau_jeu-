#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

char board[3][3];

void initializeBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = '-';
        }
    }
}

void displayBoard() {
    std::cout << "Board:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool checkWin(char player) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

bool checkDraw() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == '-') return false;
        }
    }
    return true;
}

void handleClientMove(int clientSocket, char currentPlayer) {
    char buffer[BUFFER_SIZE] = {0};
    int row, col;

    // Receive client's move
    recv(clientSocket, buffer, BUFFER_SIZE, 0);
    sscanf(buffer, "%d %d", &row, &col);

    // Update board
    if (board[row][col] == '-') {
        board[row][col] = currentPlayer;
        displayBoard();
        if (checkWin(currentPlayer)) {
            std::cout << "Player " << currentPlayer << " wins!" << std::endl;
            return;
        } else if (checkDraw()) {
            std::cout << "It's a draw!" << std::endl;
            return;
        }
    }
}

int main() {
    int serverSocket, clientSocket1, clientSocket2;
    struct sockaddr_in serverAddr, clientAddr1, clientAddr2;
    socklen_t clientAddrLen = sizeof(clientAddr1);

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 2) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for connections..." << std::endl;

    // Accept connection from first client
    if ((clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientAddr1, &clientAddrLen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "First player connected" << std::endl;

    // Accept connection from second client
    if ((clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddr2, &clientAddrLen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Second player connected" << std::endl;

    initializeBoard();
    char currentPlayer = 'X';
    bool gameOver = false;

    while (!gameOver) {
        // Send current board to first client
        send(clientSocket1, (char *)board, sizeof(board), 0);

        // Handle first client's move
        handleClientMove(clientSocket1, currentPlayer);

        // Check if game is over after first client's move
        if (checkWin(currentPlayer) || checkDraw()) {
            gameOver = true;
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

        // Send current board to second client
        send(clientSocket2, (char *)board, sizeof(board), 0);

        // Handle second client's move
        handleClientMove(clientSocket2, currentPlayer);

        // Check if game is over after second client's move
        if (checkWin(currentPlayer) || checkDraw()) {
            gameOver = true;
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    close(serverSocket);
    close(clientSocket1);
    close(clientSocket2);
    return 0;
}


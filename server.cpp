#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

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

bool checkDraw() { //check if all the board is filled (tie)
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
    /*
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
    }*/
    
    //define current player
    char currentPlayer = 'X';
    // x : player 1, O : player 2
    int client_socket = 0;
    int gameOver = 0;
    while (!gameOver) {
    	
    
    	if(currentPlayer == 'X'){
    		client_socket = clientSocket1;
    		}
    	else if(currentPlayer == 'O'){
    		client_socket = clientSocket2;
    		}
    	//send a message : It is you turn
    	//std::string message ="It is your turn.";
        send(client_socket, &gameOver, sizeof(gameOver),0);
    	
    	//send board to current player
    	send(client_socket, (char *)board, sizeof(board), 0);

        // Handle first client's move
        handleClientMove(client_socket, currentPlayer);
        
    	//check if the game is Over
    	if (checkWin(currentPlayer) || checkDraw()) {
            
            
            //send a message : You win or tie
            if(checkWin(currentPlayer)){
            	//send "You win the game !!"
            	std::string message ="You win the game !!";
            	send(client_socket, message.c_str(), message.length(),0);
            	std::cout << currentPlayer << " wins the game" << std::endl;
            	gameOver = 1;
            }
            else if(checkDraw()){
            	//send "It is a tie.";
            	std::string message ="It is a tie.";
            	send(client_socket, message.c_str(), message.length(),0);
            	std::cout << "It is a tie." << std::endl;
            	gameOver = 2;
            }
            break;
        }
        else{
        	//send a message : the other player is playing
        	std::string message ="The other player is playing.";
            	send(client_socket, message.c_str(), message.length(),0);
            	std::cout << "The other player is playing." << std::endl;
        }
    	currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    
    }
    //currentPlayer is the winner, we want to send a message to looser
    int looser_socket = 0;
    int winner_socket = 0;
    if(currentPlayer == 'X'){
    		looser_socket = clientSocket2;
    		winner_socket = clientSocket1;
    		}
    else if(currentPlayer == 'O'){
    		looser_socket = clientSocket1;
    		winner_socket = clientSocket2;
    		}
    //send a message to the player who lost to break it loop
    //std::string message ="You lost the game";
    //send(looser_socket, message.c_str(), message.length(),0);
    send(looser_socket, &gameOver, sizeof(gameOver),0);
    
    //send to the players the board at the end of the game.
    sleep(1);
    send(looser_socket, (char *)board, sizeof(board), 0);
    send(winner_socket, (char *)board, sizeof(board), 0);

    close(serverSocket);
    close(clientSocket1);
    close(clientSocket2);
    return 0;
}


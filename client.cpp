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

void get_send_move(clientSocket){
    int row, col;
    std::cout << "Enter row and column (0-2): ";
    std::cin >> row >> col;
    sprintf(buffer, "%d %d", row, col);

    // Send move to server
    send(clientSocket, buffer, strlen(buffer), 0);
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
    /*int buffer_size = 200;
    char buffer[buffer_size];*/
    std::string received_message;
    int valread;
    bool gameOver = 0;

    while (true) {
    	//receive a message :
    	valread = read(clientSocket, &gameOver,sizeof(gameOver));
    	if(!gameOver){
        	std::cout << "It is your turn." << std::endl;
        }
        else if(gameOver){
        	std::cout << "You lost the game"<< std::endl;
        	break;
        }
    	
        // Receive current board from server
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        memcpy(board, buffer, sizeof(board));

        // Display board
        displayBoard(board);

        // Get player's move and send it
        get_send_move(clientSocket);
        
        //receive the message from the server
        valread = read(clientSocket, buffer,BUFFER_SIZE);
        received_message = std::string(buffer,valread);
        if(received_message=="You win the game !!" || received_message=="It is a tie."){
        	std::cout <<received_message << std::endl;
        	break;
        }
        else if(received_message=="The other player is playing."){
        	continue;
        }
        else{
        	std::cout << "error in the game loop : "<< received_message << std::endl;
        }
    }
    //receive the board at the end of the game.
    recv(clientSocket, buffer, BUFFER_SIZE, 0);
    memcpy(board, buffer, sizeof(board));

    // Display board
    displayBoard(board);	

    close(clientSocket);
    return 0;
}


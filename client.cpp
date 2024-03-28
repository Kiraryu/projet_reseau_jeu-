#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#define PORT 8080

void displayBoard(char board[3][3]) {
    std::cout << "-------------" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " | ";
        }
        std::cout << std::endl << "-------------" << std::endl;
    }
}

void receiveMessages(int clientSocket) {
    char message[100];
    while (true) {
        // Receive messages from the server
        if (recv(clientSocket, message, sizeof(message), 0) <= 0) {
            break;
        }
        
        // Check if the message contains player information //TODO changer ca pcq ca le met n'importe ou
        if (strncmp(message, "You are", 7) == 0) {
            std::cout << message;
        } else {
            std::cout << message;
            
            // Check if the game has ended
            if (strncmp(message, "Player", 6) == 0 || strncmp(message, "It's a tie", 10) == 0) {
                break;
            }
        }
    }
}

/*
void receiveMessages(int clientSocket) {
    char message[100];
    while (true) {
        // Receive messages from the server
        if (recv(clientSocket, message, sizeof(message), 0) <= 0) {
            break;
        }
        std::cout << message;

        // Check if the game has ended
        if (strncmp(message, "Player", 6) == 0 || strncmp(message, "It's a tie", 10) == 0) {
            break;
        }
    }
}
*/ 


int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char board[3][3];
    int choice;
    int currentPlayer = 0; // Variable pour suivre le joueur actuel  //??

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }
std::cout << "socket created " << std::endl;
	
	
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        return -1;
    }
std::cout << "connected to server"<< std::endl;
	
	
    // Create a thread to receive messages from the server
    std::thread messageThread(receiveMessages, clientSocket);
	
    // Play the game
    while (true) {
        // Receive the current state of the board
        recv(clientSocket, board, sizeof(board), 0);

        // Display the board to the player
        displayBoard(board);
	//receive message from the client
	
	char message[100];
	memset(message, 0, 100);//clearing the message buffer
	
	int bytesReceived = recv(clientSocket, message, 100, 0);
	if (bytesReceived < 0) {
        std::cerr << "Error in receiving message" << std::endl;
        // Handle error
    	} 
    	else if (bytesReceived == 0) {
        std::cerr << "Connection closed by peer" << std::endl;
        // Handle connection closure
    	} 
    	else {
        // Message received successfully
        std::cout << message << std::endl;
   	}
	
        // Get player's choice only if it's their turn
        if (currentPlayer == 0) {
            std::cout << "Player 1's turn. Enter your move (1-9): ";
        } else {
            std::cout << "Player 2's turn. Enter your move (1-9): ";
        }
        std::cin >> choice;

        // Send choice to the server only if it's the player's turn
        if ((currentPlayer == 0 && choice >= 1 && choice <= 9) || (currentPlayer == 1 && choice >= 10 && choice <= 18)) {
            send(clientSocket, &choice, sizeof(choice), 0);
            currentPlayer = 1 - currentPlayer; // Switch player after valid move
        } else {
            std::cout << "Invalid move! Try again." << std::endl;  //TODO ici ca ne boucle pas et jsp pq
        }
    }

    // Join the message thread
    messageThread.join();

    close(clientSocket);
    return 0;
}




/*
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#define PORT 8080

void displayBoard(char board[3][3]) {
    std::cout << "-------------" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " | ";
        }
        std::cout << std::endl << "-------------" << std::endl;
    }
}

void receiveMessages(int clientSocket) {
    char message[100];
    while (true) {
        // Receive messages from the server
        if (recv(clientSocket, message, sizeof(message), 0) <= 0) {
            break;
        }
        std::cout << message;

        // Check if the game has ended
        if (strncmp(message, "Player", 6) == 0 || strncmp(message, "It's a tie", 10) == 0) {
            break;
        }
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char board[3][3];
    int choice;
    int currentPlayer = 0; // Variable pour suivre le joueur actuel

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

    // Create a thread to receive messages from the server
    std::thread messageThread(receiveMessages, clientSocket);
// Play the game
while (true) {
    // Receive the current state of the board
    recv(clientSocket, board, sizeof(board), 0);

    // Display the board to the player
    displayBoard(board);

    // Receive information about whose turn it is
    recv(clientSocket, &currentPlayer, sizeof(currentPlayer), 0);

    // Get player's choice only if it's their turn
    if (currentPlayer == 0 && playerNumber == 1) {
        std::cout << "Player 1's turn. Enter your move (1-9): ";
    } else if (currentPlayer == 1 && playerNumber == 2) {
        std::cout << "Player 2's turn. Enter your move (1-9): ";
    } else {
        std::cout << "Waiting for the other player's move..." << std::endl;
        continue;
    }
    std::cin >> choice;

    // Send choice to the server only if it's the player's turn
    if ((currentPlayer == 0 && playerNumber == 1 && choice >= 1 && choice <= 9) || 
        (currentPlayer == 1 && playerNumber == 2 && choice >= 10 && choice <= 18)) {
        send(clientSocket, &choice, sizeof(choice), 0);
        currentPlayer = 1 - currentPlayer; // Switch player after valid move
    } else {
        std::cout << "Invalid move! Try again." << std::endl;
    }
}

    // Play the game
    while (true) {
        // Receive the current state of the board
        recv(clientSocket, board, sizeof(board), 0);

        // Display the board to the player
        displayBoard(board);

        // Get player's choice only if it's their turn
        if (currentPlayer == 0) {
            std::cout << "Player 1's turn. Enter your move (1-9): ";
        } else {
            std::cout << "Player 2's turn. Enter your move (1-9): ";
        }
        std::cin >> choice;

        // Send choice to the server only if it's the player's turn
        if ((currentPlayer == 0 && choice >= 1 && choice <= 9) || (currentPlayer == 1 && choice >= 10 && choice <= 18)) {
            send(clientSocket, &choice, sizeof(choice), 0);
            currentPlayer = 1 - currentPlayer; // Switch player after valid move
        } else {
            std::cout << "Invalid move! Try again." << std::endl;
        }
    }

    // Join the message thread
    messageThread.join();

    close(clientSocket);
    return 0;
}


*/


/* #include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

void displayBoard(char board[3][3]) {
    std::cout << "-------------" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " | ";
        }
        std::cout << std::endl << "-------------" << std::endl;
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char board[3][3];
    int choice;
    char message[100];

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
    while (true) {
        // Receive the current state of the board
        recv(clientSocket, board, sizeof(board), 0);

        // Display the board to the player
        displayBoard(board);

        // Receive messages from the server
        recv(clientSocket, message, sizeof(message), 0);
        std::cout << message;

        // Check if the game has ended
        if (strncmp(message, "Player", 6) == 0 || strncmp(message, "It's a tie", 10) == 0) {
            return 0; // Exit the loop and end the game
        }

        // Get player's choice
        std::cout << "Enter your move (1-9): ";
        std::cin >> choice;

        // Send choice to the server
        send(clientSocket, &choice, sizeof(choice), 0);
    }

    close(clientSocket);
    return 0;
}
*/


/*
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread> // Ajout de la bibliothèque pour les threads

#define PORT 8080

void displayBoard(char board[3][3]) {
    std::cout << "-------------" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " | ";
        }
        std::cout << std::endl << "-------------" << std::endl;
    }
}

void receiveMessages(int clientSocket) {
    char message[100];
    while (true) {
        // Receive messages from the server
        if (recv(clientSocket, message, sizeof(message), 0) <= 0) {
            break;
        }
        std::cout << message;

        // Check if the game has ended
        if (strncmp(message, "Player", 6) == 0 || strncmp(message, "It's a tie", 10) == 0) {
            break;
        }
    }
}

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

    // Create a thread to receive messages from the server
    std::thread messageThread(receiveMessages, clientSocket);

    // Play the game
    while (true) {
        // Receive the current state of the board
        recv(clientSocket, board, sizeof(board), 0);

        // Display the board to the player
        displayBoard(board);

        // Get player's choice
        std::cout << "Enter your move (1-9): ";
        std::cin >> choice;

        // Send choice to the server
        send(clientSocket, &choice, sizeof(choice), 0);
    }

    // Join the message thread
    messageThread.join();

    close(clientSocket);
    return 0;
}

*/





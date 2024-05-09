# Jeu de Morpion en Réseau
Guilhem ALEX, Omar BENJELLOUN, Nikolai CHISHOLM, El-Mehdi KHARKHACHE, Martin LARROQUE

## Introduction

Ce projet est une implémentation du jeu classique de Morpion (Tic-Tac-Toe) jouable en réseau entre deux joueurs. Le jeu est développé en C++ et utilise les protocoles TCP/IP pour la communication réseau. Le jeu de morpion est un jeu de société pour deux joueurs dans lequel les joueurs alternent pour placer leur symbole (X ou O) sur une grille de 3x3. Le premier joueur à aligner trois de ses symboles horizontalement, verticalement ou en diagonale gagne la partie.

## Fonctionnalités
- Jeu Morpion pour deux joueurs.
- Communication réseau utilisant les protocoles TCP/IP.
- Serveur gérant les connexions des clients et le flux du jeu.
- Interface en ligne de commande pour une interaction simple et directe.


## Structure du Code
Le projet est structuré en trois fichiers principaux :

- `server.cpp` : Implémente toute la logique du serveur, incluant la gestion des connexions réseau et les règles du jeu.
- `client.cpp` : Gère la partie client, incluant l'interface utilisateur et la communication réseau.
- `Makefile` : Fichier utilisé pour automatiser la compilation des programmes client et serveur.

### Le fichier client
Le fichier `client.cpp` contient le code pour la partie client d'un jeu et peut être répartit comme suit : 
- Inclusion de bibliothèques.
- Définition des constantes : Le port (PORT) et la taille du buffer (BUFFER_SIZE) sont définis pour la communication réseau.
- Fonction **displayBoard** : Cette fonction affiche l'état actuel du plateau de jeu dans la console. Elle prend en entrée un tableau 2D représentant le plateau de jeu et l'imprime à l'écran.
- Fonction **get_send_move** : Cette fonction permet au joueur de saisir son coup (ligne et colonne), vérifie la validité du mouvement (la case doit être vide, représentée par le caractère '-' en ASCII), et envoie ce mouvement au serveur via un socket.
- Fonction **main** :
    - **Initialisation du socket :** Un socket TCP est créé et une connexion est établie avec le serveur à l'adresse spécifiée (127.0.0.1 ici, qui est l'adresse de boucle locale).
    - **Boucle de jeu :** Une boucle gère le flux du jeu. Elle reçoit des messages du serveur pour déterminer si c'est au tour du joueur de jouer ou si le jeu est terminé (victoire, défaite ou match nul). Selon l'état du jeu reçu du serveur, le client affiche l'état actuel du plateau, permet au joueur de faire un mouvement, et attend la réponse du serveur.
    - **Réception du plateau de jeu :** Le plateau est reçu du serveur et mis à jour localement. La fonction displayBoard est appelée pour afficher le plateau.
    - Envoi du mouvement : Le joueur fait son mouvement par la fonction get_send_move, qui est ensuite envoyé au serveur.
    - **Gestion des erreurs :** Le code gère les éventuelles erreurs de connexion ou de création de socket et termine le programme si une telle erreur survient.
    - **Fermeture du socket :** À la fin de la session de jeu, le socket est fermé proprement.

Ce client fonctionne en conjonction avec le serveur qui gère la logique du jeu, les mouvements des joueurs, la détection de la victoire ou de la défaite, et la communication entre les deux clients joueurs. Le client lui-même se concentre sur l'interaction avec l'utilisateur, la réception et l'envoi de données au serveur.

### Le fichier serveur

Le fichier `server.cpp` gère la logique serveur du jeu. Voici les détails de son fonctionnement :

- **Création du Socket :** Le serveur commence par créer un socket TCP/IP et configure son adresse pour accepter des connexions sur n'importe quelle adresse IP de la machine hôte, utilisant un port spécifique (généralement 8080).
- **Écoute des Connexions :** Une fois le socket créé et lié à l'adresse spécifiée, le serveur se met en écoute pour les connexions entrantes, configuré pour accepter au maximum deux connexions simultanément.
- **Connexion des Clients :** Le serveur accepte une connexion du premier client, puis une deuxième du second client, affichant des messages de confirmation à chaque connexion réussie.

- **Initialisation du Plateau :** Le serveur initialise le plateau de jeu à une grille de 3x3, avec toutes les cellules initialisées au caractère '-' indiquant qu'elles sont vides.
- **Boucle de Jeu :** Le serveur entre dans une boucle où il gère alternativement les mouvements de chaque joueur. Pour chaque client :
  - **Envoi de l'État du Plateau :** L'état actuel du plateau est envoyé au client dont c'est le tour.
  - **Réception et Traitement des Mouvements :** Le serveur reçoit le mouvement du client (ligne et colonne), met à jour le plateau si le mouvement est valide (la cellule doit être vide), puis vérifie s'il y a un gagnant ou si le jeu est nul.
  - **Fin du Tour :** Si un joueur gagne ou si le jeu est nul après un mouvement, le serveur annonce le résultat et sort de la boucle de jeu. Sinon, il passe au joueur suivant.

- **Fermeture des Sockets :** Après la fin du jeu, le serveur ferme les sockets de chaque client ainsi que son propre socket de serveur pour libérer les ressources.

- **Fonctions annexes :**
    - **Affichage du Plateau :** Fonction pour afficher l'état actuel du plateau dans la console du serveur.
    - **Vérification du Gagnant :** Fonctions pour vérifier si un joueur a aligné trois de ses symboles horizontalement, verticalement, ou diagonalement.
    - **Vérification du Match Nul :** Fonction pour vérifier si toutes les cellules du plateau sont remplies sans qu'un joueur ait gagné.

## Workflow
Le schéma ci-dessous résume comment fonctionne le code :
  - lors de l'exécution de server, il créé un premier socket qu'il lie à un port sur lequel il se met en écoute. 
  - le premier client est exécuté et cherche à se connecter au serveur. La connection est faite en attribuant un socket au premier client.
  - le deuxième client est exécuté et cherche à se connecter au serveur. La connection est faite en attribuant un deuxième socket au deuxième client.
  - le serveur initialise le plateau de jeu qui est définit globalement comme une liste de char à deux dimensions (3x3)
  - les variables de jeu sont définis : 
      - char currentPlayer : symbole du joueur actif (X ou O)
      - int client_socket : socket du joueur actif (initialisé à 0)
      - int gameOver : 0 si le jeu est en cours, passera à 1 lorsque le jeu est fini
  - On rentre alors dans le boucle de jeu (qui s'exécute tant que gameOver différent de 0)
      - client_socket est défini comme celui du joueur actif
      - le server envoie au client du joueur actif gameOver pour qu'il sache si la partie est fini ou pas
      - si la partie n'est pas fini, le client affiche "it's your turn", si gameOver==1, la partie est perdu pour le joueur actif, si c'est 2, le matche est nul.
      - le serveur envoie ensuite le plateau de jeu au client qui l'affiche et demande au joueur ce qu'il souhaite jouer.
      - dans la fonction get_send_move, le client vérifie la validité du jeu du joueur et l'envoie ensuite au serveur.
      - le serveur reçoit le jeu, vérifie si la partie est fini ou pas. 
          - si la partie n'est pas fini, il envoie un message au client du joueur actif que l'autre joueur va jouer, il échange les rôles des joueurs (actif devient passif et inversement), et revient au début de la boucle. Le client de l'ancien joueur actif reçoit le message et recommence sa boucle de jeu (attendant de connaître l'état de la partie en cours).
          - si la partie est fini, il envoie au joueur actif qu'il a gagné ou que c'est un match nul. Le client du joueur actif vérifie ce message et sort de sa boucle de jeu en affichant le message de match gagné ou nul à l'écran. 
  - En sortant de la boucle de jeu, le joueur actif est celui qui a gagné (si match non nul), il faut envoyer l'état de la partie (gameOver == 1 ou gameOver==2) à l'autre joueur pour qu'il sorte également de sa boucle de jeu. 
  - à la fin de la partie, le serveur attend 1 secondes (pour éviter des problèmes d'envoie de données) et envoie le plateau de jeu de la fin de partie au deux client qui l'affichent
  - les connexions sont fermés proprement

## Execution du code
Voici les étapes nécessaires pour l'execution du code :
  - compilation : dans un terminal, executer la commande "make" dans le répertoire contenant les 3 fichiers du projet
  - exécuter l'exécutable de server.cpp
  - exécuter 2 fois client pour avoir 2 joueurs. Si l'exécution a lieu sur la même machine il n'y a pas besoin de préciser l'adresse IP d'exécution du server.
  - chacun des 2 client se connecte automatiquement au server. Il suffit de suivre les indications du jeu pour que cela fonctionne.  

## Remarques :
Initialement nous souhaitions réaliser une implémentation plus complexe en utilisant notamment le multi-threading. Nous ne sommes pas parvenu à surmonter les bugs rencontrés dans la complexité du programme. De plus, cette première implémentation n'était pas modulaire et nous ne pouvions donc pas tester chaque brique indépendamment puis les assembler ensuite. Nous aurions pu choisir de résoudre tous les problèmes de cette première implémentation mais cela devenait chronophage. Nous avons donc choisi de partir sur une version plus simple avec seulement 2 fichiers : un client et un serveur, et sans utiliser le multi-threading. L'avantage est une plus grande simplicité du programme, le désavantage est de ne pas pouvoir gérer plus de 2 joueurs connectés sur le serveurs. En effet, avec la première implémentation, le but était de mettre en place un système d'invitations des joueurs connectés sur le serveur.
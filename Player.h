#pragma once
#include <string>
# include <vector>
#include <algorithm>
#include <iostream>
//#include "Game.h"
//#include "Grid.h"

class Player{

private:
	int m_socket;
	std::string m_name;
	int m_state; // 0 : just connected : 0, has invited or has been invited : 1, in game : 2.
	std::vector<Player*> m_inviting_players; //list of players that invited this player to play a game
	std::vector<Player*> m_invited_players; // list of the players this player invited to play a game
	int m_turn_number;//was to 0 but generated an error
	
public:
	Player(int socket, std::string name);
	~Player();
	

	void reject_invitation(Player* rejected_player_id); // reject the invitation you have recieved. Delete rejected_player_id of m_inviting_players de this and call delete_invitation de rejected_player_id (lorsque le client à lu les invitations, s'il en refuse il faut les retirer de notre liste des joueurs invitant, et informer ces joueurs qu'on les a retirer de notre liste pour qu'ils nous retire de leur liste des joueurs invités.)
	
	void delete_invitation(Player* refusing_player_id); // delete the refusing_player_id from m_invited_players of this (lorsqu'on refuse notre invitation, nous fait retirer l'id du joueur de la liste des joueurs qu'on avait invités)
	
	void invalidate_invitation(Player* rejected_player_id); // invalidate the invitations you have SENT to other players // call invitation_rejected of rejected_player_id with this as argument (demande à un joueur à qui on avait envoyé une invitation de l'invalider en nous la refusant)
	
	void send_invitation(Player* target_player_id);//call target_player_id->receive_invitation(this) if player available, add target_player_id to m_invited_players of this, else do not add the player  (lorsqu'on envoie une invitation, on ajoute le joueur qu'on a invité à la liste des joueurs qu'on a invité, et on demande au joueur de nous ajouter à la liste des gens qui l'ont invités. S'il n'est pas dispo, il refuse directement et on ne l'ajoute pas à la liste)
	
	int receive_invitation(Player* inviting_player_id);// check the state of this. If 2 return that this is not available. Else, add inviting_player_id to m_inviting_players of this (Lorsqu'on reçoit une invitation, si on est en jeu, on dit qu'on est pas dispo, c'est tout. Si on est dispo, on ajoute la joueur invitant dans la liste des joueurs qui nous invitent)
	
	
	void change_state(int new_state);
	void change_player_turn_number(int new_turn_nb);
	
	std::vector<Player*> get_invited_players();//done : develop get_invited_players in player.cpp
	std::vector<Player*> get_inviting_players();//done : develop get_inviting_players in player.cpp
	int get_socket();
	int get_state();
	std::string get_name();
	int get_turn_nb();
	
	
};

//void send_invitation(Player* sending_player_id); // invitation to play sent by another player who gives his player_id when calling the target's send_invitation method.

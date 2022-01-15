#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <bits/stdc++.h>
#include <vector>
#include <fstream>
using namespace std;

#define PS 11

int random(int a, int b)
{
    int x;
    x = rand() % a + b;

    return x;

}

class Users{
	protected:
	string FirstName;
	string LastName;
	string City;
	int MoneyDebt;
	int Weight;

	public:
	Users() {}
	Users(string fn, string ln, string c, int w, int d){FirstName=fn, LastName=ln, City=c, Weight=w, MoneyDebt=d; }

	void set_FirstName(string fn){FirstName=fn;}
	void set_LastName(string ln){LastName=ln;}
	void set_City(string c){City=c;}
	void set_Weight(int w){Weight=w;}
	void set_MoneyDebt(int d){MoneyDebt=d;}

	string get_FirstName(){return FirstName;}
	string get_LastName(){return LastName;}
	string get_City(){return City;}
	int get_Weight(){return Weight;}
	int get_MoneyDebt(){return MoneyDebt;}

	vector<Users *> static readUsers(vector<Users *> V_USR) {
		// opens the input file with first names, last names and cities
		ifstream fin("input.in");
		string x, y , z;
		while (fin >> x >> y >> z) {
			// create a new user
			Users *u = new Users(x, y, z, random(50, 50), random(90000, 10000));
			// add the user in the vector
			V_USR.push_back(u);
		}
		return V_USR;
	};

};

class Players : private Users
{
	private:
	int PlayerNumber;
	string Status;
	public:
    Players() : Users(){}
    Players(string fn, string ln, string c, int w, int d, int nr, string s):
        Users(fn, ln, c, w, d) {PlayerNumber = nr, Status = s;}
        string get_FirstName(){return Users::get_FirstName();}
        string get_LastName(){return Users::get_LastName();}
        string get_City(){return Users::get_City();}
        int get_Weight(){return Users::get_Weight();}
        int get_MoneyDebt(){return Users::get_MoneyDebt();}
		int get_Number(){return PlayerNumber;}
		string get_Status(){return Status;}
		void set_PlayerNumber(int number){PlayerNumber = number;}
		void set_Status(string status){Status = status;}
	
	vector<Players *> static createPlayers(vector<Users *> V_USR,
											vector<Players *> players) {
		for (int i = 10; i <= V_USR.size(); i++) {
			Users* u = V_USR[i - 1];
			// create a player with the corresponding fields
			Players* p = new Players(u->get_FirstName(), u->get_LastName(),
						u->get_City(), u->get_Weight(), u->get_MoneyDebt(),
						i - 9, "Alive");
			// add in the players vector
			players.push_back(p);
		}	
		return players;													
	}

	void printAlivePlayers(vector<Players *> players) {
		int count = 0;
		for(auto &i : players) {
			if (i->get_Status() == "Alive") {
				count++;
        		cout << i->get_FirstName() << " " << i->get_LastName()
				<< " "<<i->get_City()<<" "<<
        		i->get_Weight() << "kg "<<i->get_MoneyDebt() <<"$ "<<
				i->get_Number() << " " << i->get_Status() << endl ;
			}
    	}
		cout << "Alive players: " << count << endl;
	}
};

class Supervisors : private Users
{
private:
    string MaskShape;
public:
    Supervisors() : Users(){}
    Supervisors(string fn, string ln, string c, int w, int d, string ms):
        Users(fn, ln, c, w, d){MaskShape = ms;}
        string get_FirstName(){return Users::get_FirstName();}
        string get_LastName(){return Users::get_LastName();}
        string get_City(){return Users::get_City();}
        int get_Weight(){return Users::get_Weight();}
        int get_MoneyDebt(){return Users::get_MoneyDebt();}
        string get_MaskShape(){return MaskShape;}

	
	vector<Supervisors *> static createSupervisors(vector<Users *> V_USR,
											vector<Supervisors *> supervisors) {
		string maskShapes[3] = {"rectangle", "triangle", "circle"}; 
		for (int i = 0; i < V_USR.size(); i++) {
			// create 9 supervisors with corresponding fields
			if (i < 9) {
			Users* u = V_USR[i];
			Supervisors* s = new Supervisors(u->get_FirstName(),
							u->get_LastName(), u->get_City(),
							u->get_Weight(), u->get_MoneyDebt(),
							maskShapes[i % 3]);
			supervisors.push_back(s);
			} else {
				break;
			}
		}	
		return supervisors;													
	};

	void printSupervisors(vector<Supervisors *> supervisors) {
		cout << "Supervisors:\n";
		int nr = 0;
		for(auto &i : supervisors) {
			cout<< ++nr << ". " << i->get_FirstName() << " " << i->get_LastName()
				<< " "<<i->get_City()<<" "<<
				i->get_Weight() << "kg "<<i->get_MoneyDebt() <<"$ "<<
				i->get_MaskShape() << endl;
    	}	
	};
};

class Games {
	private:
		int Prize = 0;
	public:
		int get_Prize(){return Prize;}
		void set_Prize(int prize){Prize = prize;}

		void prize(vector<Players *> players) {
			long prize = 0;
			// adds the debt of the dead players to the prize
			for (auto &i : players) {
				if (i->get_Status() == "Dead") {
					this->Prize += i->get_MoneyDebt();
				}
			}
		}

		void static firstGame(vector<Players *> players) {
			// players with even number die
			for (auto &i : players) {
				if (i->get_Number() % 2 == 0) {
					i->set_Status("Dead"); 
				}
			}
		}

		void static secondGame(vector<Players *> players) {
			int playerNumbers[99];
			int count = 0;
			// get the alive players numbers
			for (int i = 0; i < players.size(); i++) {
				if (players[i]->get_Status() == "Alive") {
					playerNumbers[count++] = players[i]->get_Number();
				}
			}
			// randomize the player numbers array
			shuffle_array(playerNumbers, count);

			for (int i = 0; i < count; i += 8) {
				// if there are not 2 teams to duel, they will go in the next
				// round
				if (i + 8 > count) {
					break;
				}
				// weight of the first team
				int firstTeam = 0;
				// weight of the second team
				int secondTeam = 0;
				int j;
				// calculates the weight of the first team
				for (j = i; j < i + 4; j++) {
					firstTeam += players[playerNumbers[j] - 1]->get_Weight();
				}
				int k;
				// keep in mind the counter of the first team
				int auxj = j;
				// calculates the weight of the second team
				for (k = j; k < j + 4; k++) {
					secondTeam += players[playerNumbers[k] - 1]->get_Weight();
				}
				// players from the losing team die
				if (firstTeam > secondTeam) {
					for (k = auxj; k < auxj + 4; k++) {
						players[playerNumbers[k] - 1]->set_Status("Dead");
					}
				} else {
					for (j = i; j < i + 4; j++) {
						players[playerNumbers[j] - 1]->set_Status("Dead");
					}
				}
			}
		}

		void static shuffle_array(int* arr, int n) {
			// method to shuffle an int array values
			unsigned seed = 0;
			shuffle(arr, arr + n,
					default_random_engine(seed));
		}

		void static thirdGame(vector <Players *> players) {
			int playerNumbers[99];
			int count = 0;
			// get the alive players numbers
			for (auto &i : players) {
				if (i->get_Status() == "Alive") {
					playerNumbers[count++] = i->get_Number();
				}
			}
			srand(time(NULL));
			for (int i = 0; i < count; i += 2) {
				if (i + 2 > count) {
					break;
				}
				// first playe's random number
				long firstPlayer = rand();
				// second player's random number
				long secondPlayer = rand();
				// the one with a higher number dies
				if (firstPlayer < secondPlayer) {
					for (auto &j : players) {
						if (j->get_Number() == playerNumbers[i + 1]) {
							j->set_Status("Dead");
							break;
						}
					}
				} else {
					for (auto &j : players) {
						if (j->get_Number() == playerNumbers[i]) {
							j->set_Status("Dead");
							break;
						}
					}
				}
 			}
			cout << endl;
		}

		void static fourthGame(vector<Players *> players) {
			// player numbers
			int playerNumbers[99];
			int count = 0;
			// get all the player numbers
			for (auto &i : players) {
				if (i->get_Status() == "Alive") {
					playerNumbers[count++] = i->get_Number();
				}
			}
			// sort descending playern numbers
			for (int i = 0; i < count; i++) {
				for (int j = i + 1; j < count; j++) {
					if (playerNumbers[i] < playerNumbers[j]) {
						int aux = playerNumbers[i];
						playerNumbers[i] = playerNumbers[j];
						playerNumbers[j] = aux;
					}
				}
			}
			// vector to store the alive players
			vector<Players *> alivePlayers;
			cout << endl;
			srand(time(NULL));
			int rounds = 1;
			for (int i = 0; i < count; i++) {
				for (auto &j : players) {
					if (j->get_Number() == playerNumbers[i]) {
						alivePlayers.push_back(j);
					}
				}
			}
			// iterate through the alive players
			for (auto &i : alivePlayers) {
				if (i->get_Status() == "Alive" && alivePlayers.size() > 1) {
					Players *firstPlayer = i;
					for (auto &j : alivePlayers) {
						// first alive player with the number lower than the
						// first player
						if (j->get_Number() < firstPlayer->get_Number()) {
							while (1) {
								// first player's choice
								int gen = rand() % 3 + 1;
								// second player's choice
								int ken = rand() % 3 + 1;
								string choice1;
								string choice2;
								if (gen == 1) {
									choice1 = "piatra";
								} else if (gen == 2) {
									choice1 = "hartie";
								} else {
									choice1 = "foarfece";
								}
								if (ken == 1) {
									choice2 = "piatra";
								} else if (ken == 2) {
									choice2 = "hartie";
								} else {
									choice2 = "foarfece";
								}
								cout << "Runda" << rounds << endl;
								cout << "Playerul cu numarul " << firstPlayer->get_Number() << " a ales " << choice1 << endl;
								cout << "Playerul cu numarul " << j->get_Number() << " a ales " << choice2 << endl;
								if (gen == ken) {
									cout << "EGALITATE in runda " << rounds << endl;
									rounds++;
								} else {
								if (gen == 1) {
									if (ken == 2) {
										cout << "Playerul cu numarul " << firstPlayer->get_Number() << " a fost eliminat\n";
										firstPlayer->set_Status("Dead");
										firstPlayer = j;
										rounds++;
										break;
									} else if (ken == 3) {
										cout << "Playerul cu numarul " << j->get_Number() << " a fost eliminat\n";
										j->set_Status("Dead");
										j = firstPlayer;
										rounds++;
										break;
									} else {
										continue;
									}
								} else if (gen == 2) {
									if (ken == 1) {
										cout << "Playerul cu numarul " << j->get_Number() << " a fost eliminat\n";
										j->set_Status("Dead");
										j = firstPlayer;
										rounds++;
										break;
									} else if (ken == 3) {
										cout << "Playerul cu numarul " << firstPlayer->get_Number() << " a fost eliminat\n";
										firstPlayer->set_Status("Dead");
										firstPlayer = j;
										rounds++;
										break;
									} else {
										continue;
									}
								} else if (gen == 3) {
									if (ken == 1) {
										cout << "Playerul cu numarul " << firstPlayer->get_Number() << " a fost eliminat\n";
										firstPlayer->set_Status("Dead");
										firstPlayer = j;
										rounds++;
										break;
									} else if (ken == 2) {
										cout << "Playerul cu numarul " << j->get_Number() << " a fost eliminat\n";
										j->set_Status("Dead");
										j = firstPlayer;
										rounds++;
										break;
									} else {
										continue;
									}
								}
								rounds++;
								}
							}
						}
					}
				}
			}
		}
};

int main()
{
	vector<Users *> V_USR;
	Users u;
	Supervisors s;
	Players p;
	Players* winner;
	vector<Supervisors *> supervisors;
	vector<Players *> players;
    srand(time(0));
	V_USR = Users::readUsers(V_USR);
	// u.printUsers(V_USR);
	
	supervisors = Supervisors::createSupervisors(V_USR, supervisors);
	// s.printSupervisors(supervisors);

	players = Players::createPlayers(V_USR, players);
	// p.printAlivePlayers(players);

	Games games;
	cout << "\n\n================================================="
	<< "===============================================LET THE GAMES BEGIN===="
	<< "==============================================================="
	<< "=============================\n";

	cout << "\n\n\t\t\t\tFirst game is: RED LIGHT GREEN LIGHT\n";
	games.firstGame(players);
	cout << "RED LIGHT GREEN LIGHT IS OVER and the remaining players are:\n";
	p.printAlivePlayers(players);

	cout << "\n\n\t\t\t\tSecond game is: TUG OF WAR\n";
	games.secondGame(players);
	cout << "TUG OF WAR IS OVER and the remaining players are:\n";
	p.printAlivePlayers(players);

	cout << "\n\n\t\t\t\tThird game is: MARBLES\n";
	games.thirdGame(players);
	cout << "MARBLES IS OVER and the remaining players are:\n";
	p.printAlivePlayers(players);

	cout << "\n\n\t\t\t\tFourth game is: GENKEN\n";
	games.fourthGame(players);
	for (auto &i : players) {
		if (i->get_Status() == "Alive") {
			winner = i;
			break;
		}
	}
	games.prize(players);
	cout << "\n\n\n\n\t\t\t\tThe games are over and the winner is:\n" <<
		winner->get_FirstName() << " " << winner->get_LastName() << " " <<
		winner->get_City() << " " << winner->get_Weight() << " " <<
		winner->get_MoneyDebt() << " " << winner->get_Number() << " " <<
		winner->get_Status() << " who won " << games.get_Prize() << "$" << endl;
	
    return 0;
}
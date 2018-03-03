#include <stdlib.h>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include "websocket.h"

using namespace std;

#define BOARD_WITDH 500
#define BOARD_HEIGHT 500

#define BALL_RADIUS 5
#define PADDLE_LENGTH 100

#define LATENCY_TYPE_FIXED 0
#define LATENCY_TYPE_RANDOM 1
#define LATENCY_TYPE_INCREMENTAL 2

namespace latTools {
	long long min = 10, max = 100, incLat = min;
	random_device latDevice;
	default_random_engine latEngine(latDevice());
	uniform_int_distribution<long long> latDistribution(min, max);

	long long latency(int type) {
		if (type = LATENCY_TYPE_FIXED)
			return 30;
		else if (type = LATENCY_TYPE_RANDOM)
			return latDistribution(latEngine);
		else if (type = LATENCY_TYPE_INCREMENTAL) {
			incLat += 5;
			return incLat;
		}
	}

	class latQueue {
	public:
		latQueue(int type) :
			latType(type) {}

		void enqueue(string s) {
			chrono::milliseconds t = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			buff.insert(buff.begin(), make_pair(s, t.count() + latency(latType)));
		}

		pair<string, long long> dequeue() {
			chrono::milliseconds t = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
			if (t.count() >= buff.back().second) {
				pair<string, long long> result = buff.back();
				buff.pop_back();
				return result;
			}
			else
				return make_pair("",0);
		}

	private:
		vector<pair<string, long long>> buff;
		int latType;
	};
}

webSocket server;

Ball ball;
Paddle player0, player1, player2, player3;
map<int, Paddle*> IDPlayerMap;
int players = 0;

int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;

/* called when a client connects */
void openHandler(int clientID) {
	if (players < 4) {
		if (!player0.active) {
			player0 = Paddle(PADDLE_LENGTH, BOARD_WITDH/2 - PADDLE_LENGTH/2, BOARD_HEIGHT - 10, clientID, 0, true);
			IDPlayerMap[clientID] = &player0;
		}
		else if (!player1.active) {
			player1 = Paddle(PADDLE_LENGTH, BOARD_WITDH - 10, BOARD_HEIGHT/2 - PADDLE_LENGTH/2, clientID, 1, true);
			IDPlayerMap[clientID] = &player1;
		}
		else if (!player2.active) {
			player2 = Paddle(PADDLE_LENGTH, BOARD_WITDH/2 - PADDLE_LENGTH/2, 10, clientID, 2, true);
			IDPlayerMap[clientID] = &player2;
		}
		else if (!player3.active) {
			player3 = Paddle(PADDLE_LENGTH, 10, BOARD_HEIGHT/2 - PADDLE_LENGTH/2, clientID, 3, true);
			IDPlayerMap[clientID] = &player3;
		}
		
		players++;
	}
	
	if (players == 4 && !gameStarted) {
		printf("Game Start! \n");
		ball = Ball(BALL_RADIUS, BOARD_WITDH/2, BOARD_HEIGHT/2);
		gameStarted = true;
	}
}

/* called when a client disconnects */
void closeHandler(int clientID) {
	if (gameStarted) {
		players = 0;
		for (pair<int, Paddle*> p : IDPlayerMap) {
			p.second->active = false;
			if (p.second->clientID != clientID) {
				ostringstream os;
				os << "# " << IDPlayerMap[clientID]->name << " has left the game. The game is end. ";
				server.wsSend(p.second->clientID, os.str());
			}
		}
		gameStarted = false;
		printf("Game is over. Restart the server for next game...\n");
	}
	else {
		IDPlayerMap[clientID]->active = false;
		players--;
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	//Names
	if (message.substr(0, 2) == "NM") {
		IDPlayerMap[clientID]->name = message.substr(3);
	}
	else if (message == "N0") {
		server.wsSend(clientID, "N0 " + player0.name);
	}
	else if (message == "N1") {
		server.wsSend(clientID, "N1 " + player1.name);
	}
	else if (message == "N2") {
		server.wsSend(clientID, "N2 " + player2.name);
	}
	else if (message == "N3") {
		server.wsSend(clientID, "N3 " + player3.name);
	}
	//Requests of player's identification
	else if (message == "PI") {
		ostringstream os;
		os << "PI " << IDPlayerMap[clientID]->playerID;
		server.wsSend(clientID, os.str());
	}
	//Paddle positions
	else if (message.substr(0, 2) == "P0")
		player0.x = stoi(message.substr(3));
	else if (message.substr(0, 2) == "P1")
		player1.y = stoi(message.substr(3));
	else if (message.substr(0, 2) == "P2")
		player2.x = stoi(message.substr(3));
	else if (message.substr(0, 2) == "P3")
		player3.y = stoi(message.substr(3));
}

/* called once per select() loop */
void periodicHandler() {
	if (gameStarted)
	{
		static clock_t next = clock() + interval_clocks;
		clock_t current = clock();
		if (current >= next) {
			ball.x += ball.x_speed;
			ball.y += ball.y_speed;

			//Hitting left or right wall
			if (ball.x - ball.radius < 0) {
				ball.x = ball.radius;
				ball.x_speed = fabs(ball.x_speed);
			}
			else if (ball.x + ball.radius > BOARD_WITDH) {
				ball.x = BOARD_WITDH - ball.radius;
				ball.x_speed = -fabs(ball.x_speed);
			}
				
			//Hitting top or bottom wall
			if (ball.y - ball.radius < 0) {
				ball.y = ball.radius;
				ball.y_speed = fabs(ball.y_speed);
			}
			else if (ball.y + ball.radius > BOARD_HEIGHT) {
				ball.y = BOARD_HEIGHT - ball.radius;
				ball.y_speed = -fabs(ball.y_speed);
			}

			//Hitting on player0's paddle
			if (ball.y + ball.radius > BOARD_HEIGHT - 10) {
				if (ball.x >= player0.x && ball.x <= player0.x + player0.length) {
					ball.y = BOARD_HEIGHT - 10 - ball.radius;
					ball.y_speed = -fabs(ball.y_speed);
					player0.score++;
				}
			}
			//Hitting on player1's paddle
			if (ball.x + ball.radius > BOARD_WITDH - 10) {
				if (ball.y >= player1.y && ball.y <= player1.y + player1.length) {
					ball.x = BOARD_WITDH - 10 - ball.radius;
					ball.x_speed = -fabs(ball.x_speed);
					player1.score++;
				}
			}
			//Hitting on player2's paddle
			if (ball.y - ball.radius < 10) {
				if (ball.x >= player2.x && ball.x <= player2.x + player2.length) {
					ball.y = 10 + ball.radius;
					ball.y_speed = fabs(ball.y_speed);
					player2.score++;
				}
			}
			//Hitting on player3's paddle
			if (ball.x - ball.radius < 10) {
				if (ball.y >= player3.y && ball.y <= player3.y + player3.length) {
					ball.x = 10 + ball.radius;
					ball.x_speed = fabs(ball.x_speed);
					player3.score++;
				}
			}

			ostringstream os;
			os << "BP " << ball.x << " " << ball.y << "\n";
			os << "P0 " << player0.x << "\n";
			os << "P1 " << player1.y << "\n";
			os << "P2 " << player2.x << "\n";
			os << "P3 " << player3.y << "\n";
			os << "S0 " << player0.score << "\n";
			os << "S1 " << player1.score << "\n";
			os << "S2 " << player2.score << "\n";
			os << "S3 " << player3.score << "\n";

			for (pair<int, Paddle*> p : IDPlayerMap) {
				if (p.second->name == "")
					server.wsSend(p.second->clientID, os.str() + "NM\n");
				else
					server.wsSend(p.second->clientID, os.str());
			}
		}
		next = clock() + interval_clocks;
	}
}

int main(int argc, char *argv[]){
    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler);

    /* start the Pong Game server, listen to Local IP and port '8000' */
    server.startServer(8000);

    return 1;
}

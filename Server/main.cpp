#include <stdlib.h>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"

using namespace std;

#define BOARD_WITDH 500
#define BOARD_HEIGHT 500

#define BALL_RADIUS 5
#define PADDLE_LENGTH 100

webSocket server;

Ball ball;
Paddle player0;
int activeID;
bool inGame = false;

int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;

/* called when a client connects */
void openHandler(int clientID){
	ball = Ball(BALL_RADIUS, BOARD_WITDH/2, BOARD_HEIGHT/2);
	player0 = Paddle(PADDLE_LENGTH, BOARD_WITDH/2 - PADDLE_LENGTH/2, BOARD_HEIGHT - 10);
	activeID = clientID;
	inGame = true;
	printf("Player %d has begun the game\n", clientID);
}

/* called when a client disconnects */
void closeHandler(int clientID){
	activeID = NULL;
	inGame = false;
	printf("Game is over. Waiting for next connection...\n");
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	//Name of player0
	if (message.substr(0,2) == "N0"){
		player0.name = message.substr(3);
	}
	//Paddle position of player0
	if (message.substr(0,2) == "P0"){
		player0.x = stoi(message.substr(3));
	}
}

/* called once per select() loop */
void periodicHandler() {
	if (inGame)
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

			ostringstream os;
			os << "BP " << ball.x << " " << ball.y << "\n";
			os << "S0 " << player0.score << "\n";
			server.wsSend(activeID, os.str());
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

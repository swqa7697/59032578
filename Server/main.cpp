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

webSocket server;

Ball ball;
Paddle player0;
int activeID;
bool inGame = false;

int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;

/* called when a client connects */
void openHandler(int clientID){
	ball = Ball(5, 250.0, 250.0);
	player0 = Paddle(100, 5, 200, 490);
	activeID = clientID;
	inGame = true;
	printf("Player %d has begun the game\n", clientID);
}

/* called when a client disconnects */
void closeHandler(int clientID){
	activeID = NULL;
	printf("Game is over. Waiting for next connection...\n");
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	ostringstream os;

	if (message.substr(0,2) == "N0"){
		player0.name = message.substr(3);
	}
	if (message.substr(0,2) == "P0"){
		player0.posx = stoi(message.substr(3));
	}
}

/* called once per select() loop */
void periodicHandler() {
	if (inGame)
	{
		static clock_t next = clock() + interval_clocks;
		clock_t current = clock();
		if (current >= next) {
			ball.posx += ball.x_speed;
			ball.posy += ball.y_speed;

			if ((ball.x_speed < 0 && ball.posx - ball.radius < 0) ||
				(ball.x_speed > 0 && ball.posx + ball.radius > 500 )) {
				ball.x_speed *= -1;
			}

			if ((ball.y_speed < 0 && ball.posy - ball.radius < 0) ||
				(ball.y_speed > 0 && ball.posy + ball.radius > 500)) {
				ball.y_speed *= -1;
			}

			if (ball.y_speed > 0 && ball.posy + ball.radius > 490) {
				if (ball.posx > player0.posx && ball.posx < player0.posx + player0.w) {
					ball.y_speed *= -1;
					player0.score++;
				}
			}

			ostringstream os;
			os << "BP " << ball.posx << " " << ball.posy << "\n";
			os << "S0 " << player0.score << "\n";
			cout << os.str();
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

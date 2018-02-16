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

Ball ball = Ball(5, 250.0, 250.0);
Paddle player0 = Paddle(100, 5, 200, 490);
int activeID;

/* called when a client connects */
void openHandler(int clientID){
	activeID = clientID;
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
		
	}
}

/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + (float)0.3;
    time_t current = time(NULL);
    if (current >= next){
		ball.posx += ball.x_speed;
		ball.posy += ball.y_speed;

		if ((ball.x_speed < 0 && ball.posx <= 5) ||
			(ball.x_speed > 0 && ball.posx >= 495)) {
			ball.x_speed = -ball.x_speed;
		}

		if (ball.y_speed < 0) {
			if (ball.posy <= 5) {
				ball.y_speed = -ball.y_speed;
			}
		}
		else {
			if (player0.posy >= ball.posy) {
				//collision distance
				float d = player0.posy - ball.posy;
				//time
				float t = d / ball.y_speed;
				if (t < 0) {
					t = -t;
				}
				//x translate
				float x = ball.x_speed * t + (ball.posx - ball.x_speed);
				if (x >= player0.posx && x <= player0.posx + player0.w)
				{
					ball.y_speed = -ball.y_speed;
					player0.score++;
				}
			}
			if (ball.posy < 5)
				ball.y_speed = -ball.y_speed;
		}
        ostringstream os;
		os << "BP " << ball.posx << " " << ball.posy << "\n";
		os << "P0 " << player0.posx << " " << player0.posy << "\n";
		os << "S0 " << player0.score << "\n";
		server.wsSend(activeID, os.str());
		}
		next = time(NULL) + (float)0.3;
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

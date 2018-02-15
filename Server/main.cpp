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

Ball ball = Ball(5, 250, 250);
Paddle player(100, 5, 200, 490);

/* called when a client connects */
void openHandler(int clientID){  
	ostringstream os;
	os << "One player has begun the game";
    vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() > 1) {
		for (int i = 2; i < clientIDs.size() + 1; ++i) {			
			server.wsSend(clientIDs[i], os.str());
			server.wsClose(i);
		}
	}
}

/* called when a client disconnects */
void closeHandler(int clientID){
	
    ostringstream os;
	
	
    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID)
            server.wsSend(clientIDs[i], os.str());
    }
	
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	ostringstream os;
	vector<int> clientIDs = server.getClientIDs();
	if (message[0] == 'N') {
		for (int i = 0; i < message.size(); ++i) {
			if (message[i] == ' ') {
				i++;
				for (int i = 0; i < message.size(); ++i) {
					player.name += message[i];
				}
				break;
			}
		}
	}
	if (message[0] == 'P') {

	}
	if (message == "l") {
		
		player.posx = fmin(0, player.posx - player.speed);
	
	}
	if (message == "r") {
		player.posx = fmax(500 - player.posx, player.posx + player.speed);
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
			if (player.posy >= ball.posy) {
				//collision distance
				int d = player.posy - ball.posy;
				//time
				int t = d / ball.y_speed;
				if (t < 0) {
					t = -t;
				}
				//x translate
				int x = ball.x_speed * t + (ball.posx - ball.x_speed);
				if (x >= player.posx && x <= player.posx + player.w)
				{
					ball.y_speed = -ball.y_speed;
					player.score++;
				}
			}
			if (ball.posy < 5)
				ball.y_speed = -ball.y_speed;
		}
        ostringstream os;
		os << "BP " << ball.posx << " " << ball.posy << "\n";
		os << "P0 " << player.posx << " " << player.posy << "\n";
		os << "S0 " << player.score << "\n";
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) {
			server.wsSend(clientIDs[i], os.str());
		}
		next = time(NULL) + (float)0.3;
    }
}

int main(int argc, char *argv[]){
	
	ball.posx = 250;
	ball.posy = 250;
	ball.x_speed = 8;
	ball.y_speed = 8;

	player.posx = 250;
	player.posy = 500;
    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
    server.setPeriodicHandler(periodicHandler);

    /* start the Pong Game server, listen to Local IP and port '8000' */
    server.startServer(8000);

    return 1;
}

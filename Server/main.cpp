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

std::pair<int, int> pos;

struct Ball {
	int w = 10;
	int h = 10;
	int posx;
	int posy;
	int speed;
	int acc;
	int vx = 0;
	int vy = 0;
	int maxangle = 45;

	Ball() {}


	Ball(int positionx, int positiony, int initialspeed, int angle) {
		posx = positionx;
		posy = positiony;
		speed = initialspeed;
		velocity(angle);
	}

	void velocity(float angle) {
		float radians = angle * 3.14159265 / 180;		
		vx = speed * (int)sin(radians);		
		vy = -speed * (int)cos(radians);	
	}

	void paddleCollision(int paddleX, int paddleWidth) {
		float paddleCenterX = paddleX + paddleWidth / 2;

		float disFromCenter = (posx + (w / 2)) - paddleCenterX; //ballCenterX - paddleCenterX

		float ratio = disFromCenter / (paddleWidth / 2);

		velocity(maxangle * ratio);
	}
};


/*
struct Paddle {
	int w = 100;
	int h = 5;
	int posx;
	int posy;
	int speed = 1; 
	int score = 0;
	string name = "player1";

	Paddle() {}

	Paddle(int Width, int Height, int positionX, int positionY, string n) {
		w = Width;
		h = Height;
		posx = positionX;
		posy = positionY;
		name = n;
	}
};
*/

Ball ball;

//Paddle player;



/* called when a client connects */
void openHandler(int clientID){  
    vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() > 1) {
		for (int i = 2; i < clientIDs.size() + 1; ++i) {
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

	if (message == "l") {
		player.posx = fmin(0, player.posx - player.speed);
	
	}
	if (message == "r") {
		player.posx = fmax(pos.first - player.posx, player.posx + player.speed);
	}

}

/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + (float)0.3;
    time_t current = time(NULL);
    if (current >= next){
		ball.posx += ball.vx;
		ball.posy += ball.vy;

		if ((ball.vx < 0 && ball.posx < 0) ||
			(ball.vx > 0 && ball.posx + ball.w * 0.5 > pos.first)) {
			ball.vx = -ball.vx;
		}


		if (ball.vy < 0) {
			if (ball.posy < 0) {
				ball.vy = -ball.vy;
			}
		}
		else {
			if (player.posy < ball.posy + ball.h * 0.5) {
				//collision distance
				int d = ball.posy + ball.h * 0.5 - player.posy;
				//time
				int t = d / ball.vy;
				//x translate
				int x = ball.vx * t + (ball.posx - ball.vx);
				if (x >= player.posx && x + ball.w <= player.posx + player.w)
				{
					ball.vy = -ball.vy;
					player.score++;
				}
			}
			if (ball.posy > pos.second)
				ball.vy = -ball.vy;
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
	pos.first = 1024;
	pos.second = 768;
	ball.posx = pos.first * 0.5;
	ball.posy = pos.second * 0.5;
	ball.vx = 8;
	ball.vy = 8;

	player.posx = pos.first * 0.5;
	player.posy = pos.second;
    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
    server.setPeriodicHandler(periodicHandler);

    /* start the Pong Game server, listen to Local IP and port '8000' */
    server.startServer(8888);

    return 1;
}

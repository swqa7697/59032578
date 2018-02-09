#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"

using namespace std;

webSocket server;

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
	/*
    ostringstream os;
    os << "Stranger " << clientID << " has leaved.";

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID)
            server.wsSend(clientIDs[i], os.str());
    }
	*/
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	server.wsSend(clientID, message);

}

/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + (float)0.3;
    time_t current = time(NULL);
    if (current >= next){
        ostringstream os;
		//Deprecated ctime API in Windows 10
		char timecstring[26];
		ctime_s(timecstring, sizeof(timecstring), &current);
		string timestring(timecstring);
        timestring = timestring.substr(0, timestring.size() - 1);
        os << timestring;

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], os.str());

        next = time(NULL) + (float)0.3;
    }
}

int main(int argc, char *argv[]){
    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
  //  server.setPeriodicHandler(periodicHandler);

    /* start the Pong Game server, listen to Local IP and port '8000' */
    server.startServer(8000);

    return 1;
}

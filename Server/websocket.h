#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#ifdef __linux__
#include <arpa/inet.h>
#elif _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <random>
#include <cmath>

using namespace std;

typedef void (*nullCallback)();
typedef void (*defaultCallback)(int);
typedef void (*messageCallback)(int, string);

#define WS_FIN 128
#define WS_MASK 128

#define WS_OPCODE_CONTINUATION 0
#define WS_OPCODE_TEXT         1
#define WS_OPCODE_BINARY       2
#define WS_OPCODE_CLOSE        8
#define WS_OPCODE_PING         9
#define WS_OPCODE_PONG         10

#define WS_PAYLOAD_LENGTH_16 126
#define WS_PAYLOAD_LENGTH_63 127

#define WS_READY_STATE_CONNECTING 0
#define WS_READY_STATE_OPEN       1
#define WS_READY_STATE_CLOSING    2
#define WS_READY_STATE_CLOSED     3

#define WS_STATUS_NORMAL_CLOSE             1000
#define WS_STATUS_GONE_AWAY                1001
#define WS_STATUS_PROTOCOL_ERROR           1002
#define WS_STATUS_UNSUPPORTED_MESSAGE_TYPE 1003
#define WS_STATUS_MESSAGE_TOO_BIG          1004

#define WS_STATUS_TIMEOUT 3000

#define WS_TIMEOUT_RECV 10
#define WS_TIMEOUT_PONG 5

#define INTERVAL_MS 10

#define BALL_SPEED_SCALE 2

namespace
{
	random_device device;
	default_random_engine engine(device());
	uniform_real_distribution<double> distribution(-1.0, 1.0);

	float rd(){
		return distribution(engine);
	}

	void scaledNormalize(double &x, double &y){
		double z = sqrt(x*x + y*y);

		x /= z;
		x *= BALL_SPEED_SCALE;

		y /= z;
		y *= BALL_SPEED_SCALE;
	}
}

class wsClient{
public:
    wsClient(int _socket, in_addr _addr){
        socket = _socket;
        MessageBuffer.clear();
        ReadyState = WS_READY_STATE_CONNECTING;
        LastRecvTime = time(NULL);
        PingSentTime = 0;
        CloseStatus = 0;
        addr = _addr;
        FramePayloadDataLength = -1;
        FrameBytesRead = 0;
        FrameBuffer.clear();
        MessageOpcode = 0;
        MessageBufferLength = 0;
    }

	std::string getTimeStamp();
	int calculateLatency(std::string timeData);

	int networkDelay;
    int socket;                            // client socket
    string MessageBuffer;                  // a blank string when there's no incoming frames
    int ReadyState;                        // between 0 and 3
    time_t LastRecvTime;                   // set to time() when the client is added
    time_t PingSentTime;                   // 0 when the server is not waiting for a pong
    int CloseStatus;                       // close status that wsOnClose() will be called with
    in_addr addr;
    int FramePayloadDataLength;            // length of a frame's payload data.
    int FrameBytesRead;                    // amount of bytes read for a frame, reset to 0 when all frame data has been read
    string FrameBuffer;                    // joined onto end as a frame's data comes in, reset to blank string when all frame data has been read
    unsigned char MessageOpcode;           // stored by the first frame for fragmented messages, default value is 0
    size_t MessageBufferLength;            // the payload data length of MessageBuffer
	clock_t timeElapsed;                   // the amount of time passed since playing the game. default value should be 0 before the start of the game
};

struct Ball {
	double radius;
	double x;
	double y;
	double x_speed;
	double y_speed;

	Ball():
		radius(0.5), x(0), y(0), x_speed(rd()), y_speed(rd()) {
		scaledNormalize(x_speed, y_speed);
	}

	Ball(double radius, double positionx, double positiony):
		radius(radius), x(positionx), y(positiony), x_speed(rd()), y_speed(rd()) {
		scaledNormalize(x_speed, y_speed);
	}
};

struct Paddle {
	string name;
	int length;
	int x;
	int y;
	int score;

	Paddle():
		name(""), length(0), x(0), y(0), score(0) {}

	Paddle(int Length, int posX, int posY):
		name(""), length(Length), x(posX), y(posY), score(0) {}
};

class webSocket{
public:
    webSocket(){
        callOnOpen = NULL;
        callOnClose = NULL;
        callOnMessage = NULL;
        callPeriodic = NULL;
    }

    void setOpenHandler(defaultCallback callback);
    void setCloseHandler(defaultCallback callback);
    void setMessageHandler(messageCallback callback);
    void setPeriodicHandler(nullCallback callback);
    void startServer(int port);
    void stopServer();
    bool wsSend(int clientID, string message, bool binary = false);
    void wsClose(int clientID);
    vector<int> getClientIDs();
    string getClientIP(int clientID);
private:
    vector<wsClient *> wsClients;
    map<int, int> socketIDmap;
    fd_set fds;
    int fdmax;
    int listenfd;

    void wsCheckIdleClients();
    bool wsSendClientMessage(int clientID, unsigned char opcode, string message);
    void wsSendClientClose(int clientID, unsigned short status = -1);
    bool wsCheckSizeClientFrame(int clientID);
    void wsRemoveClient(int clientID);
    bool wsProcessClientMessage(int clientID, unsigned char opcode, string data, int dataLength);
    bool wsProcessClientFrame(int clientID);
    bool wsBuildClientFrame(int clientID, const char *buffer, int bufferLength);
    bool wsProcessClientHandshake(int clientID, char *buffer);
    bool wsProcessClient(int clientID, char *buffer, int bufferLength);
    int wsGetNextClientID();
    void wsAddClient(int socket, in_addr ip);

    defaultCallback callOnOpen;
    defaultCallback callOnClose;
    messageCallback callOnMessage;
    nullCallback callPeriodic;
};

std::vector<std::string> &parseTime(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> parseTime(const std::string &s, char delim);
#endif

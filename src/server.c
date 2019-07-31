#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <SDL_net.h>
#include <unistd.h>
#include "protocol.h"
#include "color.h"

static volatile int keepRunning = 1;
static UDPsocket sock;
static int channel;
static UDPpacket *packet = NULL;
static UDPpacket *sendPacket = NULL;

void intHandler(int sig) {
	keepRunning = 0;
	printf(COLOR_BOLD_ITALIC COLOR_YELLOW "\nFound SIGINT signal, stopping server...\n" COLOR_RESET);
}

int send_packet(int clean) {
	int sent;
	sent = SDLNet_UDP_Send(sock, packet->channel, sendPacket);
	if(!sent) {
		fprintf(stderr, "Error sending packet: %s\n", SDLNet_GetError());
	}
	if(clean == 1) {
		memset(sendPacket->data, 0, PACKET_SIZE);
	}
	return sent;
}

int main(int argc, const char* argv[]) {
	signal(SIGINT, intHandler);

	int port = 6666;

	if(SDL_Init(0) == -1) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if(SDLNet_Init() == -1) {
		fprintf(stderr, "Error initializing SDLNet: %s\n", SDLNet_GetError());
		return EXIT_FAILURE;
	}

	sock = SDLNet_UDP_Open(port);

	if(!sock) {
		fprintf(stderr, "Error opening socket at port %d: %s\n", 6666, SDLNet_GetError());
		return EXIT_FAILURE;
	}

	IPaddress address;

	SDLNet_ResolveHost(&address, NULL, port);

	channel = SDLNet_UDP_Bind(sock, -1, &address);

	if(channel == -1) {
		fprintf(stderr, "Error binding socket: %s\n", SDLNet_GetError());
		return EXIT_FAILURE;
	}


	UDPpacket *packet;
	UDPpacket *sendPacket;

	packet = SDLNet_AllocPacket(PACKET_SIZE);
	sendPacket = SDLNet_AllocPacket(PACKET_SIZE);
	memset(packet->data, 0, PACKET_SIZE);
	memset(sendPacket->data, 0, PACKET_SIZE);

	int nrecv;

	while (keepRunning) {
		nrecv = SDLNet_UDP_Recv(sock, packet);
		if(nrecv == 0) {
			usleep(1);
			continue;
		}
		else if(nrecv == -1) {
			fprintf(stderr, "Error receiving a packet: %s\n", SDLNet_GetError());
		}

		char *host;

		if(!(host = (char*)SDLNet_ResolveIP(&packet->address))) {
			fprintf(stderr, "Error resolving ip: %s\n", SDLNet_GetError());
			return EXIT_FAILURE;
		}

		printf(COLOR_GREEN "Received packet from %s:%d with size: %d\n" COLOR_RESET,
				host, packet->address.port, nrecv);
	}

	SDLNet_FreePacket(packet);
	SDLNet_FreePacket(sendPacket);
	SDLNet_UDP_Unbind(sock, channel);
	SDLNet_UDP_Close(sock);
	SDLNet_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}

#include "netclient.h"
#include <SDL_net.h>
#include <engine/logger.h>

static UDPsocket sock;
static int channel = 0;
static UDPpacket *packet = NULL;
static UDPpacket *sendPacket = NULL;
static unsigned int packetSize = 0;
static Uint8 *packetData = NULL;
static Uint8 *sendPacketData = NULL;


int netclient_init(unsigned int packet_size) {
	if(SDLNet_Init() == -1) {
		log_write(LOG_ERROR, "Error initializing SDLNet: %s\n", SDLNet_GetError());
		return 0;
	}
	packetSize = packet_size;

	sock = SDLNet_UDP_Open(0);

	if(!sock) {
		log_write(LOG_ERROR, "Error opening socket: %s\n", SDLNet_GetError());
		return 0;
	}

	packet = SDLNet_AllocPacket(packet_size);
	sendPacket = SDLNet_AllocPacket(packet_size);
	memset(packet->data, 0, packet_size);
	memset(sendPacket->data, 0, packet_size);

	packetData = packet->data;
	sendPacketData = sendPacket->data;

	return 1;
}

void netclient_quit() {
	SDLNet_FreePacket(packet);
	SDLNet_FreePacket(sendPacket);
	SDLNet_UDP_Unbind(sock, channel);
	SDLNet_UDP_Close(sock);
	SDLNet_Quit();
}

int netclient_connect(const char *ip, unsigned short port) {
	IPaddress address;

	SDLNet_ResolveHost(&address, ip, port);

	channel = SDLNet_UDP_Bind(sock, -1, &address);

	if(channel == -1) {
		log_write(LOG_ERROR, "Error binding socket: %s\n", SDLNet_GetError());
		return 0;
	}

	return 1;
}

void netclient_disconnect() {
	SDLNet_UDP_Unbind(sock, channel);
}

int netclient_send(int clean) {
	int sent;
	sent = SDLNet_UDP_Send(sock, channel, sendPacket);
	if(!sent) {
		log_write(LOG_ERROR, "Error sending packet: %s\n", SDLNet_GetError());
	}
	if(clean == 1) {
		memset(sendPacket->data, 0, packetSize);
		sendPacket->len = 0;
	}
	return sent;
}

int netclient_recv() {
	int nrecv;
	nrecv = SDLNet_UDP_Recv(sock, packet);
	if(nrecv == -1) {
		log_write(LOG_ERROR, "Error receiving a packet: %s\n", SDLNet_GetError());
	}

	if(nrecv > 0) {
		log_write(LOG_DEBUG, "Received packet with size: %d\n", nrecv);
	}
	return nrecv;
}

void netclient_write_int(int value) {
}

void netclient_write_short(short value);
void netclient_write_str(const char *str);
void netclient_write_raw(void *data, int size);

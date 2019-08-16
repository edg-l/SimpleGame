#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include "color.h"
#include "list.h"
#include "protocol.h"
#include <graphics/renderer.h>
#include "logger.h"
#include "config.h"
#include <ui/button.h>

static UDPsocket sock;
static int port = 6666;
static int channel;
static UDPpacket *packet = NULL;
static UDPpacket *sendPacket = NULL;
static int width = 1024;
static int height = 768;

int send_packet(int clean) {
	int sent;
	sent = SDLNet_UDP_Send(sock, channel, sendPacket);
	if(!sent) {
		log_write(LOG_ERROR, "Error sending packet: %s\n", SDLNet_GetError());
	}
	if(clean == 1) {
		memset(sendPacket->data, 0, PACKET_SIZE);
		sendPacket->len = 0;
	}
	return sent;
}

int main(int argc, const char* argv[]) {

	if(!render_init(1024, 768, "SimpleGame")) {
		log_write(LOG_ERROR, "Error creating renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if(SDLNet_Init() == -1) {
		log_write(LOG_ERROR, "Error initializing SDLNet: %s\n", SDLNet_GetError());
		return EXIT_FAILURE;
	}

	sock = SDLNet_UDP_Open(0);

	if(!sock) {
		log_write(LOG_ERROR, "Error opening socket at port %d: %s\n", 6666, SDLNet_GetError());
		return EXIT_FAILURE;
	}

	IPaddress address;

	SDLNet_ResolveHost(&address, "localhost", port);

	channel = SDLNet_UDP_Bind(sock, -1, &address);

	if(channel == -1) {
		log_write(LOG_ERROR, "Error binding socket: %s\n", SDLNet_GetError());
		return EXIT_FAILURE;
	}

	UDPpacket *packet;
	UDPpacket *sendPacket;

	packet = SDLNet_AllocPacket(PACKET_SIZE);
	sendPacket = SDLNet_AllocPacket(PACKET_SIZE);
	memset(packet->data, 0, PACKET_SIZE);
	memset(sendPacket->data, 0, PACKET_SIZE);

	int nrecv;

	render_text_color(100, 255, 255, 255);
	int versionTextID = render_create_cached_text(18, STYLE_REGULAR, VERSION_STR, NULL, NULL);

	SDL_Color fg = {0, 255, 0, 255};
	SDL_Color bg = {200,255,255,255};
	Button *button = button_create(200, 100, 2, "Hello", DEFAULT_FG, DEFAULT_BG);
	button->x = 40;
	button->y = 40;

	while(1) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				goto cleanup;
			}
		}

		// Update

		nrecv = SDLNet_UDP_Recv(sock, packet);
		if(nrecv == -1) {
			log_write(LOG_ERROR, "Error receiving a packet: %s\n", SDLNet_GetError());
		}

		if(nrecv > 0) {
			log_write(LOG_INFO, "Received packet with size: %d\n", nrecv);
		}

		// Render
		render_color(46, 46, 46, 255);
		render_clear();

		render_cached_text(versionTextID, 5, height - 30);
		render_button(button);

		render_present();
		SDL_Delay(1);
	}

cleanup:
	SDLNet_FreePacket(packet);
	SDLNet_FreePacket(sendPacket);
	SDLNet_UDP_Unbind(sock, channel);
	SDLNet_UDP_Close(sock);
	SDLNet_Quit();
	render_quit();
	return EXIT_SUCCESS;
}

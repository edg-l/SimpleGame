#ifndef ENGINE_NETCLIENT_H
#define ENGINE_NETCLIENT_H


int netclient_init(unsigned int packet_size);
void netclient_quit();

int netclient_connect(const char *ip, unsigned short port);
void netclient_disconnect();

int netclient_send(int clean);
int netclient_recv();

void netclient_write_int(int value);
void netclient_write_short(short value);
void netclient_write_str(const char *str);
void netclient_write_raw(void *data, int size);

int netclient_read_int(int value);
short netclient_read_short(short value);

#endif

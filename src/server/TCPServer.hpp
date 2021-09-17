#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/signalfd.h>
#include <unordered_map>

#include "utils/utils.hpp"
#include "utils/Constants.hpp"


struct Client
{
	int fd_;
	int port_;
	char address_[sizeof("xxx.xxx.xxx.xxx") + 1];
	FILE *fout_;
};


class TCPServer
{
private:
	int tcp_fd_;
	int epoll_fd_;
	int signal_fd_;
	bool running_;
	std::unordered_map<int, Client> clients_;

public:
	TCPServer();
	~TCPServer();
	void init();
	void run();

private:
	void init_epoll();
	void init_socket();
	void init_signals();

	void epoll_add(int epoll_fd, int fd, uint32_t events);
	void epoll_delete(int epoll_fd, int fd);

	friend class Handler;
};
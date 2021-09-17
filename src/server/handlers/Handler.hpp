#pragma once

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>

#include "utils/utils.hpp"
#include "TCPServer.hpp"


class Handler {
public:
	static void handler_signal_event(int fd, TCPServer& instance);
	static void handler_client_event(int fd, TCPServer& instance);
	static void handler_accept_new_client(int fd, TCPServer& instance);
};
#include "Handler.hpp"


void Handler::handler_signal_event(int fd, TCPServer& instance)
{
	struct signalfd_siginfo info;
	int bytes = read(fd, &info, sizeof(info));
	ASSERT(bytes != sizeof(info), "read failed");

	unsigned sig = info.ssi_signo;
	unsigned user = info.ssi_uid;

	if (sig == SIGINT)
		printf("Got SIGINT from user %u\n", user);
	if (sig == SIGHUP)
		printf("Got SIGHUP from user %u\n", user);
	if (sig == SIGTERM)
		printf("Got SIGTERM from user %u\n", user);

	// TODO: Clear server resources
	instance.running_ = false;
	for (auto client : instance.clients_)
		close(client.second.fd_);
}

void Handler::handler_client_event(int fd, TCPServer& instance)
{
	size_t ret;
	ssize_t recv_ret;
	char buffer[1024];
	Client client = instance.clients_[fd];

	memset(buffer, '-', sizeof(buffer));
	recv_ret = recv(fd, buffer, sizeof(buffer), 0);
	ASSERT(recv_ret < 0, "recv failed");

	if (recv_ret == 0)
		goto close_conn;

	// Testing:
	// buffer[recv_ret] = '\0';
	// fprintf(client.fout_, "%s", buffer);

	ret = fwrite(buffer, 1, (size_t) recv_ret, client.fout_);
	ASSERT(ret != (size_t) recv_ret, "fwrite failed");

	return;

	// TODO: send this task to thread pool

close_conn:
	printf("Client %s:%d has closed its connection\n",
    		client.address_, client.port_);
	instance.epoll_delete(instance.epoll_fd_, fd);
	instance.clients_.erase(fd);
	fclose(client.fout_);
	client.fout_ = NULL;
	close(client.fd_);
}

void Handler::handler_accept_new_client(int fd, TCPServer& instance)
{
	int client_fd, client_port;
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	const char *ip;
	char ip_buf[sizeof("xxx.xxx.xxx.xxx")];
	Client new_client;

	memset(&addr, 0, sizeof(addr));
	client_fd = accept(fd, (struct sockaddr *) &addr, &addr_len);
	ASSERT(client_fd < 0, "accept failed");

	client_port = ntohs(addr.sin_port);
	ip = convert_addr_ntop(&addr, ip_buf);
	ASSERT(!ip, "convert_addr_ntop failed");

	new_client.fout_ = fopen(std::string(
		"./recv/" + std::to_string(client_port) +  "client.txt").
		c_str(), "w");
	ASSERT(!new_client.fout_, "fopen failed");

	new_client.fd_ = client_fd;
	new_client.port_ = client_port;
	memset(new_client.address_, '\0', sizeof(new_client.address_));
	memcpy(new_client.address_, ip_buf, sizeof(ip_buf));

        instance.epoll_add(instance.epoll_fd_, client_fd, EPOLLIN | EPOLLPRI);
	instance.clients_[client_fd] = new_client;

	printf("New client (%s:%d) accepted...\n",
		new_client.address_, new_client.port_);
}
#include "TCPServer.hpp"
#include "handlers/Handler.hpp"


TCPServer::TCPServer()
{
	epoll_fd_  = -1;
	signal_fd_ = -1;
	tcp_fd_    = -1;
	running_   = false;
}

TCPServer::~TCPServer()
{
	if (epoll_fd_ != -1)
		close(epoll_fd_);
	if (signal_fd_ != -1)
		close(signal_fd_);
	if (tcp_fd_ != -1)
		close(tcp_fd_);
}

void TCPServer::init()
{
	init_epoll();
	init_signals();
	init_socket();
	running_ = true;
}

void TCPServer::run()
{
	int ret;
	int timeout   = Constants::get_instance().EPOLL_TIMEOUT;
	int maxevents = Constants::get_instance().EPOLL_MAX_EVENTS;
	struct epoll_event events[maxevents];

	printf("Entering event loop...\n");

	while (running_) {
		/*
		 * Sleep on `epoll_wait` and the kernel will wake up the
		 * server when event comes to my monitored file descriptors,
		 * or when the timeout reached.
		 */
		ret = epoll_wait(epoll_fd_, events, maxevents, timeout);
		ASSERT(ret == -1, "epoll_wait failed");

		if (ret == 0) {
			/* `epoll_wait` reached its timeout */
			printf("No event within %d milliseconds\n", timeout);

			continue;
		}

		for (int i = 0; i < ret; i++) {
			int fd = events[i].data.fd;

			if (fd == signal_fd_) {
				/* Event: signal to stop the server, clear resources. */
				Handler::handler_signal_event(fd, *this);

				break;
			}

			if (fd == tcp_fd_) {
				/* Event: new client is connecting... */
				Handler::handler_accept_new_client(fd, *this);

				continue;
            		}

			/* Event(s) from client, call `recv()` to read it. */
			Handler::handler_client_event(fd, *this);
        	}
    	}
}

void TCPServer::init_epoll()
{
	int epoll_fd;

	printf("Initializing epoll_fd...\n");

	/* The epoll_create argument is ignored on modern Linux. */
    	epoll_fd = epoll_create(Constants::get_instance().EPOLL_SIZE_CREATE);
	ASSERT(epoll_fd < 0, "epoll_create failed");

	epoll_fd_ = epoll_fd;
}

void TCPServer::init_socket()
{
	int ret;
	int tcp_fd;

	struct sockaddr_in addr;
	socklen_t addr_len    = sizeof(addr);
	const char *bind_addr = Constants::get_instance().SERVER_ADDRESS;
	uint16_t bind_port    = Constants::get_instance().SERVER_PORT;

	printf("Creating TCP socket...\n");

	tcp_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	ASSERT(tcp_fd < 0, "socket failed");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_port        = bind_port;
	addr.sin_addr.s_addr = inet_addr(bind_addr);

	ret = bind(tcp_fd, (struct sockaddr *) &addr, addr_len);
	ASSERT(ret < 0, "bind failed");

	ret = listen(tcp_fd, Constants::get_instance().BACKLOG_LISTEN);
	ASSERT(ret < 0, "listen failed");

	printf("Listening on %s:%u...\n", bind_addr, bind_port);

	/*
	 * Add `tcp_fd` to epoll monitoring.
	 *
	 * If epoll returned tcp_fd in `events` then a client is
	 * trying to connect to us.
	 */
	epoll_add(epoll_fd_, tcp_fd, EPOLLIN | EPOLLPRI);

	tcp_fd_ = tcp_fd;
}

void TCPServer::init_signals()
{
	int ret;
        int signal_fd;
	sigset_t sigset;

	printf("Initializing signal_fd...\n");

        /* Create a sigset of all the interested in signals. */
        ret = sigemptyset(&sigset);
	ASSERT(ret, "sigemptyset failed");

        ret = sigaddset(&sigset, SIGINT);
	ASSERT(ret, "sigaddset failed");

        ret = sigaddset(&sigset, SIGHUP);
        ASSERT(ret, "sigaddset failed");

	ret = sigaddset(&sigset, SIGTERM);
        ASSERT(ret, "sigaddset failed");

        /* Block the signals in order for signalfd to receive them. */
	ret = sigprocmask(SIG_BLOCK, &sigset, NULL);
	ASSERT(ret, "sigprocmask failed");

        /* Create the signalfd */
        signal_fd = signalfd(-1, &sigset, 0);
	ASSERT(signal_fd == -1, "signalfd failed");

	/*
	 * Add `signal_fd` to epoll monitoring.
	 *
	 * If epoll returned signal_fd in `events` then a signal
	 * has been triggered.
	 */
	epoll_add(epoll_fd_, signal_fd, EPOLLIN);

	signal_fd_ = signal_fd;
}

void TCPServer::epoll_add(int epoll_fd, int fd, uint32_t events)
{
	int ret;
	struct epoll_event event;

	memset(&event, 0, sizeof(struct epoll_event));
	event.events  = events;
	event.data.fd = fd;

	ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
	ASSERT(ret < 0, "epoll_ctl(EPOLL_CTL_ADD) failed");
}

void TCPServer::epoll_delete(int epoll_fd, int fd)
{
	int ret;

	ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	ASSERT(ret < 0, "epoll_ctl(EPOLL_CTL_DEL) failed");
}
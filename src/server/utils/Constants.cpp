#include "Constants.hpp"


std::unique_ptr<Constants> Constants::constants_ = nullptr;

Constants::Constants():
	SERVER_ADDRESS("127.0.0.1"),
	SERVER_PORT(7000),
	BACKLOG_LISTEN(4096),
	EPOLL_SIZE_CREATE(255),
	EPOLL_TIMEOUT(3000),
	EPOLL_MAX_EVENTS(1000)
{
}

Constants::~Constants()
{
}

Constants& Constants::get_instance()
{
	if (constants_ == nullptr) {
		constants_.reset(new Constants());
	}

	return *constants_;
}
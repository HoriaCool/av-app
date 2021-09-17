#pragma once

#include <memory>
#include <sys/epoll.h>


/**
 * Singleton for configurable constants *
 * 
 * The Constants class defines the `get_instance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class Constants
{
public:
	const char* const SERVER_ADDRESS;/* in milliseconds */
	const uint16_t    SERVER_PORT;
	const int         BACKLOG_LISTEN;      /* queue limit for incoming connections*/
	const int         EPOLL_SIZE_CREATE;   /* for old Linux kernels */
	const int         EPOLL_TIMEOUT;       /* in milliseconds */
	const int         EPOLL_MAX_EVENTS;

	/**
	 * The Singleton's constructor should always be private to prevent direct
	 * construction calls with the `new` operator.
	 */
protected:
	Constants();

	static std::unique_ptr<Constants> constants_;

public:
	~Constants();

	/**
	 * Singletons should not be cloneable.
	 */
	Constants(Constants &other) = delete;

	/**
	 * Singletons should not be assignable.
	 */
	void operator=(const Constants &) = delete;

	/**
	 * This is the static method that controls the access to the singleton
	 * instance. On the first run, it creates a singleton object and places it
	 * into the static field. On subsequent runs, it returns the client existing
	 * object stored in the static field.
	 */
	static Constants& get_instance();
};
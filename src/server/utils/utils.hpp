#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <exception>


#define ASSERT(assertion, call_description)					\
	do {									\
		if (assertion) {						\
			fprintf(stderr, "(%s, %d, %s): (errno=%d) %s\n",	\
				__FILE__, __LINE__, call_description,		\
				errno, strerror(errno));			\
			/* exit(errno); */					\
			std::terminate();					\
		}								\
	} while (0)


const char* convert_addr_ntop(struct sockaddr_in *addr, char *src_ip_buf);
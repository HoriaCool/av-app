#include <arpa/inet.h>
#include "utils.hpp"


const char* convert_addr_ntop(struct sockaddr_in *addr, char *src_ip_buf)
{
	const char *ret = NULL;
	in_addr_t saddr = addr->sin_addr.s_addr;

	ret = inet_ntop(AF_INET, &saddr, src_ip_buf, sizeof("xxx.xxx.xxx.xxx"));
	ASSERT(ret == NULL, "inet_ntop failed");

	return ret;
}
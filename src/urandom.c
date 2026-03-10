/**
 * Shared /dev/urandom reader used by linux, darwin, and posix_fallback backends.
 * Thread-safe: no static state; opens and closes the fd per call.
 */
#include "sources.h"

#ifndef _WIN32

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define URANDOM_PATH "/dev/urandom"

tr_error tr_fill_urandom(void *buf, size_t len)
{
	int fd = open(URANDOM_PATH, O_RDONLY);
	if (fd < 0)
		return TR_ERR_IO;
	size_t total = 0;
	unsigned char *p = (unsigned char *)buf;
	while (total < len) {
		ssize_t n = read(fd, p + total, len - total);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			close(fd);
			return TR_ERR_IO;
		}
		if (n == 0) {
			close(fd);
			return TR_ERR_IO;
		}
		total += (size_t)n;
	}
	close(fd);
	return TR_OK;
}

#else
tr_error tr_fill_urandom(void *buf, size_t len)
{
	(void)buf; (void)len;
	return TR_ERR_NOT_SUPPORTED;
}
#endif

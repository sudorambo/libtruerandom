/**
 * POSIX fallback: /dev/urandom.
 * Used when not Windows and not Linux/Darwin (e.g. other BSD), or as fallback.
 * On Windows, stub returns TR_ERR_NOT_SUPPORTED.
 */
#include "sources.h"

#ifndef _WIN32

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define URANDOM_PATH "/dev/urandom"

tr_error tr_fill_posix_fallback(void *buf, size_t len)
{
	if (len == 0)
		return TR_OK;
	int fd = open(URANDOM_PATH, O_RDONLY);
	if (fd < 0)
		return TR_ERR_IO;
	size_t total = 0;
	unsigned char *p = (unsigned char *)buf;
	while (total < len) {
		ssize_t n = read(fd, p + total, len - total);
		if (n <= 0) {
			close(fd);
			return (errno == EINTR) ? tr_fill_posix_fallback(buf, len) : TR_ERR_IO;
		}
		total += (size_t)n;
	}
	close(fd);
	return TR_OK;
}

#else
tr_error tr_fill_posix_fallback(void *buf [[maybe_unused]], size_t len [[maybe_unused]])
{
	return TR_ERR_NOT_SUPPORTED;
}
#endif

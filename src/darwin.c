/**
 * Darwin/BSD backend: getentropy(2) with fallback to /dev/urandom.
 * Stub returns TR_ERR_NOT_SUPPORTED when not __APPLE__ or BSD.
 */
#include "sources.h"
#include <stddef.h>

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __APPLE__
#include <sys/random.h>
#endif

#define URANDOM_PATH "/dev/urandom"

static tr_error fill_urandom(void *buf, size_t len)
{
	int fd = open(URANDOM_PATH, O_RDONLY);
	if (fd < 0)
		return TR_ERR_IO;
	size_t total = 0;
	unsigned char *p = (unsigned char *)buf;
	while (total < len) {
		ssize_t n = read(fd, p + total, len - total);
		if (n <= 0) {
			close(fd);
			return (errno == EINTR) ? fill_urandom(buf, len) : TR_ERR_IO;
		}
		total += (size_t)n;
	}
	close(fd);
	return TR_OK;
}

tr_error tr_fill_darwin(void *buf, size_t len)
{
	if (len == 0)
		return TR_OK;
#ifdef __APPLE__
	if (getentropy(buf, len) == 0)
		return TR_OK;
	if (errno != ENOSYS)
		return TR_ERR_IO;
#endif
	return fill_urandom(buf, len);
}

#else
tr_error tr_fill_darwin(void *buf, size_t len)
{
	(void)buf;
	(void)len;
	return TR_ERR_NOT_SUPPORTED;
}
#endif

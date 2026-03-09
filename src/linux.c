/**
 * Linux backend: getrandom(2) with fallback to /dev/urandom.
 * Thread-safe: no static state.
 */
#ifdef __linux__

#include "sources.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __has_include
#  if __has_include(<sys/random.h>)
#    include <sys/random.h>
#    define HAVE_GETRANDOM 1
#  endif
#endif
#ifndef HAVE_GETRANDOM
/* getrandom() may exist on kernel 3.17+ but not in libc headers; use syscall */
#  include <sys/syscall.h>
#  if defined(SYS_getrandom)
#    define HAVE_GETRANDOM 1
#    define getrandom(buf, len, flags) ((ssize_t)syscall(SYS_getrandom, (buf), (size_t)(len), (unsigned int)(flags)))
#  endif
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

tr_error tr_fill_linux(void *buf, size_t len)
{
	if (len == 0)
		return TR_OK;

#ifdef HAVE_GETRANDOM
	size_t total = 0;
	unsigned char *p = (unsigned char *)buf;
	while (total < len) {
		ssize_t n = getrandom(p + total, len - total, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			/* ENOSYS = kernel too old; fall back to /dev/urandom */
			if (errno == ENOSYS)
				return fill_urandom(buf, len);
			return TR_ERR_IO;
		}
		total += (size_t)n;
	}
	return TR_OK;
#else
	return fill_urandom(buf, len);
#endif
}

#endif /* __linux__ */

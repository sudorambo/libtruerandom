/**
 * Linux backend: getrandom(2) with fallback to /dev/urandom.
 * Thread-safe: no static state.
 */
#ifdef __linux__

#include "sources.h"
#include <errno.h>
#include <stddef.h>

#ifdef __has_include
#  if __has_include(<sys/random.h>)
#    include <sys/random.h>
#    define HAVE_GETRANDOM 1
#  endif
#endif
#ifndef HAVE_GETRANDOM
#  include <sys/syscall.h>
#  include <unistd.h>
#  if defined(SYS_getrandom)
#    define HAVE_GETRANDOM 1
#    define getrandom(buf, len, flags) ((ssize_t)syscall(SYS_getrandom, (buf), (size_t)(len), (unsigned int)(flags)))
#  endif
#endif

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
			if (errno == ENOSYS)
				return tr_fill_urandom(buf, len);
			return TR_ERR_IO;
		}
		total += (size_t)n;
	}
	return TR_OK;
#else
	return tr_fill_urandom(buf, len);
#endif
}

#endif /* __linux__ */

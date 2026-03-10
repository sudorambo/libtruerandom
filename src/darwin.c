/**
 * Darwin/BSD backend: getentropy(2) with fallback to /dev/urandom.
 * Stub returns TR_ERR_NOT_SUPPORTED when not __APPLE__ or BSD.
 */
#include "sources.h"
#include <stddef.h>

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)

#include <errno.h>

#ifdef __APPLE__
#include <sys/random.h>
#endif

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
	return tr_fill_urandom(buf, len);
}

#else
tr_error tr_fill_darwin(void *buf [[maybe_unused]], size_t len [[maybe_unused]])
{
	return TR_ERR_NOT_SUPPORTED;
}
#endif

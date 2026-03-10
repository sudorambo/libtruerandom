/**
 * POSIX fallback: /dev/urandom.
 * Used when not Windows and not Linux/Darwin (e.g. other BSD), or as fallback.
 * On Windows, stub returns TR_ERR_NOT_SUPPORTED.
 */
#include "sources.h"

#ifndef _WIN32

tr_error tr_fill_posix_fallback(void *buf, size_t len)
{
	if (len == 0)
		return TR_OK;
	return tr_fill_urandom(buf, len);
}

#else
tr_error tr_fill_posix_fallback(void *buf, size_t len)
{
	(void)buf; (void)len;
	return TR_ERR_NOT_SUPPORTED;
}
#endif

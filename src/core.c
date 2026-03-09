/**
 * Core API: parameter validation and backend dispatch.
 * Thread-safe: no unprotected global state; backends are stateless or thread-safe.
 */
#include <truerandom/truerandom.h>
#include "sources.h"
#include <stddef.h>
#include <string.h>

typedef tr_error (*fill_fn)(void *buf, size_t len);

static fill_fn get_fill(void)
{
#if defined(_WIN32)
	return tr_fill_windows;
#elif defined(__linux__)
	return tr_fill_linux;
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
	return tr_fill_darwin;
#else
	return tr_fill_posix_fallback;
#endif
}

tr_error tr_bytes(void *buf, size_t len)
{
	if (len == 0)
		return TR_OK;
	if (buf == NULL)
		return TR_ERR_PARAM;
	return get_fill()(buf, len);
}

tr_error tr_uint32(uint32_t *out)
{
	if (out == NULL)
		return TR_ERR_PARAM;
	return tr_bytes(out, sizeof(*out));
}

tr_error tr_uint64(uint64_t *out)
{
	if (out == NULL)
		return TR_ERR_PARAM;
	return tr_bytes(out, sizeof(*out));
}

tr_error tr_range(uint32_t *out, uint32_t min_inclusive, uint32_t max_inclusive)
{
	if (out == NULL || min_inclusive > max_inclusive)
		return TR_ERR_PARAM;
	tr_error err = tr_bytes(out, sizeof(*out));
	if (err != TR_OK)
		return err;
	uint32_t range = max_inclusive - min_inclusive + 1;
	uint32_t r = *out;
	if (range == 0) /* overflow: full [0,UINT32_MAX] */
		*out = r;
	else
		*out = min_inclusive + (r % range);
	return TR_OK;
}

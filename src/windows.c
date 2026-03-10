/**
 * Windows backend: BCryptGenRandom.
 * Stub returns TR_ERR_NOT_SUPPORTED when not on Windows.
 */
#include "sources.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

tr_error tr_fill_windows(void *buf, size_t len)
{
	if (len == 0)
		return TR_OK;
	NTSTATUS st = BCryptGenRandom(nullptr, (PUCHAR)buf, (ULONG)len, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!BCRYPT_SUCCESS(st))
		return TR_ERR_IO;
	return TR_OK;
}
#else
tr_error tr_fill_windows(void *buf [[maybe_unused]], size_t len [[maybe_unused]])
{
	return TR_ERR_NOT_SUPPORTED;
}
#endif

/**
 * Internal backend contract for libtruerandom.
 * Each backend implements a fill function: write exactly len bytes of entropy to buf,
 * or return an error (no partial write). Not installed; for use by core.c only.
 */
#ifndef TRUERANDOM_SOURCES_H
#define TRUERANDOM_SOURCES_H

#include <stddef.h>
#include "../include/truerandom/truerandom.h"

/* Backend fill functions: (buf, len) -> tr_error; on success exactly len bytes written. */
[[nodiscard]] tr_error tr_fill_linux(void *buf, size_t len);
[[nodiscard]] tr_error tr_fill_windows(void *buf, size_t len);
[[nodiscard]] tr_error tr_fill_darwin(void *buf, size_t len);
[[nodiscard]] tr_error tr_fill_posix_fallback(void *buf, size_t len);

#endif /* TRUERANDOM_SOURCES_H */

/**
 * libtruerandom — Portable C API for true random (entropy) bytes.
 * Single header; include as: #include <truerandom/truerandom.h>
 */
#ifndef TRUERANDOM_TRUERANDOM_H
#define TRUERANDOM_TRUERANDOM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Result of any library call that can fail.
 */
typedef enum tr_error {
	TR_OK = 0,
	TR_ERR_PARAM,         /* Invalid parameter (e.g. null buffer when len>0, or min>max for tr_range) */
	TR_ERR_NOT_SUPPORTED, /* Platform or entropy source not supported */
	TR_ERR_IO,            /* I/O or system error while obtaining entropy */
	TR_ERR_NO_ENTROPY     /* No entropy available (optional) */
} tr_error;

/**
 * Fill a buffer with entropy bytes.
 *
 * Returns: TR_OK on success (exactly len bytes written); TR_ERR_PARAM if buf is NULL and len>0;
 *          TR_ERR_NOT_SUPPORTED, TR_ERR_IO, or TR_ERR_NO_ENTROPY on failure.
 * On failure, buffer contents are unspecified.
 */
[[nodiscard]] tr_error tr_bytes(void *buf, size_t len);

/**
 * Write a single random uint32_t derived from entropy to *out.
 *
 * Returns: TR_OK on success; TR_ERR_PARAM if out is NULL; or other errors.
 */
[[nodiscard]] tr_error tr_uint32(uint32_t *out);

/**
 * Write a single random uint64_t derived from entropy to *out.
 *
 * Returns: TR_OK on success; TR_ERR_PARAM if out is NULL; or other errors.
 */
[[nodiscard]] tr_error tr_uint64(uint64_t *out);

/**
 * Write a value in [min_inclusive, max_inclusive] derived from entropy to *out.
 * Unbiased only when (max_inclusive - min_inclusive + 1) is a power of two.
 *
 * Returns: TR_OK on success; TR_ERR_PARAM if out is NULL or min_inclusive > max_inclusive; or other errors.
 */
[[nodiscard]] tr_error tr_range(uint32_t *out, uint32_t min_inclusive, uint32_t max_inclusive);

#ifdef __cplusplus
}
#endif

#endif /* TRUERANDOM_TRUERANDOM_H */

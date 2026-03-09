# Data Model: libtruerandom Core Library

**Feature**: 001-libtruerandom-core  
**Date**: 2025-03-09

The library is stateless and has no persistent domain data. This document describes the public types, the semantics of inputs and outputs, and the internal backend contract.

## Public Types

### tr_error (enumeration)

Represents the result of any library call that can fail.

| Value | Meaning |
|-------|--------|
| TR_OK | Success; requested bytes or value have been written. |
| TR_ERR_PARAM | Invalid parameter (e.g. null buffer when length &gt; 0, or min &gt; max for tr_range). |
| TR_ERR_NOT_SUPPORTED | Platform or entropy source not supported (e.g. unsupported OS or build). |
| TR_ERR_IO | I/O or system error while obtaining entropy. |
| TR_ERR_NO_ENTROPY | No entropy available (optional; when distinguishable from generic I/O). |

No other public types. Buffer length uses standard `size_t`; output values use `uint32_t` or `uint64_t` as specified per function.

## Input/Output Semantics

### Entropy buffer

- **Caller-owned**: Caller provides a pointer and length; library never allocates the buffer.
- **On success**: Exactly `len` bytes are written; no more, no less.
- **On failure**: Contents are unspecified; library does not guarantee partial writes.

### Single-integer outputs (tr_uint32, tr_uint64, tr_range)

- **Caller-owned**: Caller provides a pointer to a single `uint32_t` or `uint64_t`.
- **On success**: One value is written, derived from entropy; for tr_range, value is in [min_inclusive, max_inclusive].
- **On failure**: Output is unspecified; library returns an error.

### Validation rules

- **tr_bytes**: `buf` may be null only if `len == 0`. If `len > 0` and `buf` is null, return TR_ERR_PARAM.
- **tr_uint32 / tr_uint64**: `out` must not be null; otherwise TR_ERR_PARAM.
- **tr_range**: `out` must not be null; `min_inclusive` must be ≤ `max_inclusive`; otherwise TR_ERR_PARAM.

## Internal Backend Contract (not public)

Used only by the core layer to invoke the selected platform backend.

- **Operation**: Fill a buffer with entropy bytes.
- **Inputs**: Buffer pointer (non-null when length &gt; 0), length (number of bytes to write).
- **Success**: Backend writes exactly that many bytes and returns a success indicator.
- **Failure**: Backend returns an error indicator; may map to TR_ERR_IO, TR_ERR_NOT_SUPPORTED, or TR_ERR_NO_ENTROPY; no partial write.
- **Allocation**: Backend does not allocate the buffer; caller (core) provides it.
- **Thread safety**: Backend must be safe to call from multiple threads (e.g. no unprotected static state for the fill path).

No persistent state is required; backends may use one-time initialization (e.g. opening /dev/urandom) in a thread-safe way.

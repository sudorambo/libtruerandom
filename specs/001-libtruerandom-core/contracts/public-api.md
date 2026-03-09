# Public API Contract: libtruerandom

**Feature**: 001-libtruerandom-core  
**Date**: 2025-03-09  
**Header**: `truerandom/truerandom.h` (include path: `#include <truerandom/truerandom.h>`)

This contract defines the public interface that implementations must provide and that consumers and tests may rely on. Full semantics are in docs/SPECIFICATION.md.

## Types

### tr_error

Enumeration of return codes. All functions that can fail return `tr_error`.

| Symbol | Value (implementation-defined) | Meaning |
|--------|--------------------------------|--------|
| TR_OK | — | Success. |
| TR_ERR_PARAM | — | Invalid parameter. |
| TR_ERR_NOT_SUPPORTED | — | Platform or entropy source not supported. |
| TR_ERR_IO | — | I/O or system error. |
| TR_ERR_NO_ENTROPY | — | No entropy available (optional). |

## Functions

### tr_bytes

```c
tr_error tr_bytes(void *buf, size_t len);
```

- **Precondition**: If `len > 0`, `buf` must be non-null and point to at least `len` bytes of writable memory.
- **Postcondition (success)**: Exactly `len` bytes of entropy have been written to `buf`; returns `TR_OK`.
- **Postcondition (failure)**: Returns one of `TR_ERR_PARAM`, `TR_ERR_NOT_SUPPORTED`, `TR_ERR_IO`, `TR_ERR_NO_ENTROPY`; contents of `buf` are unspecified.
- **Edge case**: If `len == 0`, `buf` may be null; returns `TR_OK` without writing.

---

### tr_uint32

```c
tr_error tr_uint32(uint32_t *out);
```

- **Precondition**: `out` must be non-null.
- **Postcondition (success)**: `*out` contains a value derived from entropy; returns `TR_OK`.
- **Postcondition (failure)**: Returns `TR_ERR_PARAM` (if `out` is null) or another documented error; `*out` is unspecified.

---

### tr_uint64

```c
tr_error tr_uint64(uint64_t *out);
```

- **Precondition**: `out` must be non-null.
- **Postcondition (success)**: `*out` contains a value derived from entropy; returns `TR_OK`.
- **Postcondition (failure)**: Returns `TR_ERR_PARAM` (if `out` is null) or another documented error; `*out` is unspecified.

---

### tr_range

```c
tr_error tr_range(uint32_t *out, uint32_t min_inclusive, uint32_t max_inclusive);
```

- **Precondition**: `out` must be non-null; `min_inclusive` ≤ `max_inclusive`.
- **Postcondition (success)**: `*out` is in the closed interval [min_inclusive, max_inclusive] and derived from entropy; returns `TR_OK`. Unbiased only when (max_inclusive - min_inclusive + 1) is a power of two.
- **Postcondition (failure)**: Returns `TR_ERR_PARAM` (if `out` is null or min &gt; max) or another documented error; `*out` is unspecified.

## Guarantees

- **Thread safety**: All functions are safe to call concurrently from multiple threads.
- **No partial success**: On failure, no function reports success after writing only part of the requested data.
- **No silent fallback**: The library does not silently fall back to weak or non-entropy randomness; failures are reported via return code.
- **Stateless**: No initialization or teardown is required; no handles to close.

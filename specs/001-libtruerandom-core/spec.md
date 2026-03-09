# Feature Specification: libtruerandom Core Library

**Feature Branch**: `001-libtruerandom-core`  
**Created**: 2025-03-09  
**Status**: Draft  
**Input**: Implement libtruerandom C library for true random (entropy) bytes per docs/SPECIFICATION.md

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Obtain raw entropy bytes (Priority: P1)

As a C application or library author, I need to fill a buffer with true random (entropy) bytes from the operating system or hardware so that I can get cryptographic-quality or otherwise unpredictable data for nonces, tokens, session IDs, or seeding a PRNG—without writing platform-specific code or depending on large third-party stacks.

**Why this priority**: Filling a buffer with entropy is the core capability; all other features build on it.

**Independent Test**: Call the “fill buffer” API with a fixed-size buffer, assert success and that the buffer is not filled with a single constant byte; repeat to confirm bytes vary. Delivers usable entropy for downstream use.

**Acceptance Scenarios**:

1. **Given** a valid buffer and length, **When** the caller requests N bytes of entropy, **Then** the library writes exactly N bytes and returns success.
2. **Given** a valid buffer and length zero, **When** the caller requests 0 bytes, **Then** the library returns success without writing.
3. **Given** an unsupported platform or unavailable entropy source, **When** the caller requests bytes, **Then** the library returns a documented error and does not fill the buffer with non-entropy data.

---

### User Story 2 - Obtain single random integers (Priority: P2)

As a C application author, I need to obtain a single random 32-bit or 64-bit value, or a value in a specified integer range, derived from entropy so that I can avoid manual buffer allocation and conversion for common use cases (e.g. random IDs, dice rolls).

**Why this priority**: Convenience wrappers reduce integration effort and errors; they depend on the core fill-buffer capability.

**Independent Test**: Call the single-integer APIs (e.g. uint32, uint64, value-in-range), assert success and that returned values are not a constant; for range API, assert value lies within [min, max] inclusive. Delivers correct, entropy-derived integers.

**Acceptance Scenarios**:

1. **Given** a valid output pointer, **When** the caller requests a single random uint32 or uint64, **Then** the library writes one value derived from entropy and returns success.
2. **Given** a valid output pointer and an inclusive range [min, max] with min ≤ max, **When** the caller requests a value in range, **Then** the library writes a value in [min, max] derived from entropy and returns success.
3. **Given** a null pointer where non-null is required, **When** the caller invokes any of these functions, **Then** the library returns an invalid-parameter error.

---

### User Story 3 - Predictable error handling (Priority: P3)

As a deployer or integrator, I need the library to return explicit, documented error codes so that I can distinguish unsupported platform, I/O or system failure, invalid arguments, and (where applicable) temporary unavailability of entropy, and handle them appropriately (e.g. logging, fallback UI, or abort).

**Why this priority**: Clear failure semantics are required for safe integration; the library must never silently succeed with weak or non-entropy data.

**Independent Test**: Trigger each documented failure mode (e.g. null buffer when length &gt; 0, invalid range, or mocked backend failure) and assert the returned error code matches the documented value. Delivers reliable error handling for callers.

**Acceptance Scenarios**:

1. **Given** invalid parameters (e.g. null buffer with non-zero length, or min &gt; max for range), **When** the caller invokes the API, **Then** the library returns the documented invalid-parameter error.
2. **Given** a supported platform where the entropy source fails (e.g. I/O error), **When** the caller requests bytes, **Then** the library returns a documented I/O or no-entropy error and does not return success.
3. **Given** an unsupported platform or build, **When** the caller requests bytes, **Then** the library returns a documented unsupported-platform error.

---

### Edge Cases

- **Null buffer with non-zero length**: Library returns invalid-parameter error; no bytes written.
- **Zero length**: Allowed; success returned with no bytes written.
- **Range API with min &gt; max**: Library returns invalid-parameter error.
- **Unsupported platform or build**: Library returns unsupported-platform error; no fallback to weak randomness.
- **Entropy source temporarily unavailable or I/O failure**: Library returns I/O or no-entropy error; no partial success (either full request satisfied or error).
- **Concurrent calls from multiple threads**: All public APIs behave correctly under concurrent use (thread-safe).

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The library MUST provide a single function that fills a caller-provided buffer with exactly the requested number of entropy bytes and returns a success or error result.
- **FR-002**: The library MUST provide functions to obtain a single random 32-bit unsigned integer and a single 64-bit unsigned integer derived from entropy.
- **FR-003**: The library MUST provide a function to obtain a random value in a specified closed integer interval [min, max] (inclusive) derived from entropy.
- **FR-004**: The library MUST return explicit, documented error codes for at least: invalid parameter, unsupported platform, I/O or system error, and (where distinguishable) no entropy available.
- **FR-005**: The library MUST NOT silently fall back to weak or non-entropy randomness on any failure; on failure it MUST return an error and leave buffer contents unspecified.
- **FR-006**: The library MUST support at least Linux, Windows, and macOS with one API; backend selection is by build target, not runtime switch.
- **FR-007**: The library MUST be usable without mandatory third-party dependencies beyond the C standard and platform entropy APIs for the core path.
- **FR-008**: All public API functions MUST be safe to call from multiple threads concurrently.
- **FR-009**: On success, the library MUST produce exactly the requested bytes or value; on failure, it MUST NOT indicate partial success (no “partial bytes written” success).

### Key Entities

- **Entropy buffer**: A contiguous region of memory provided by the caller, filled with entropy bytes on success; length is specified by the caller.
- **Error code**: A documented enumeration or set of values indicating success or failure reason (invalid parameter, unsupported, I/O, no entropy).
- **Random value**: A single integer (32-bit, 64-bit, or in a specified range) derived from entropy and written to a caller-provided location.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Application authors can fill a buffer with entropy bytes and receive a clear success or failure result without writing platform-specific code.
- **SC-002**: Application authors can obtain a single random integer (fixed width or in a closed range) with one call and correct error handling.
- **SC-003**: Callers can distinguish unsupported platform, I/O failure, and invalid arguments via documented, stable error codes.
- **SC-004**: The library builds and its test suite runs on at least Linux, Windows, and macOS (or documented subset). “Supported platform” means a platform for which a backend is implemented; the test suite runs on each such platform (see implementation plan and tasks).
- **SC-005**: Integration for the core use case requires only the library and the C standard plus platform APIs; no optional crypto or third-party stacks are required for the primary entropy path.

## Assumptions

- The underlying OS and hardware entropy sources (e.g. getrandom, /dev/urandom, BCryptGenRandom) are trusted for the intended use; the library does not certify or measure entropy quality.
- Callers use a C (or C++) environment that can link a small C library and include a single public header.
- Minimum platform versions (e.g. kernel, OS release) are documented in the technical specification (docs/SPECIFICATION.md) where relevant.
- Optional CPU RNG (e.g. RDRAND/RDSEED) may be used as supplement or fallback where specified; it is not the sole source when an OS entropy source is available.

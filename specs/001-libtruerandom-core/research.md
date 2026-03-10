# Research: libtruerandom Core Library

**Feature**: 001-libtruerandom-core  
**Date**: 2025-03-09

All technical choices are defined in docs/SPECIFICATION.md; this document records the consolidated decisions and rationale for the implementation plan.

## Language and Standard

- **Decision**: C23 (ISO/IEC 9899:2024).
- **Rationale**: Spec mandates C23 for portability and a well-defined baseline; C23 compiler support is now widely available.
- **Alternatives considered**: C17 (previous baseline); C11 (wider tooling support but spec targets C23).

## Build System

- **Decision**: CMake as primary build system (minimum version to be documented, e.g. 3.10 or 3.14).
- **Rationale**: Cross-platform (Linux, Windows, macOS), standard for C libraries; options for BUILD_SHARED_LIBS, install, and tests.
- **Alternatives considered**: Make only (less portable for Windows); Meson (not specified).

## Platform Backends

- **Decision**: One backend per platform family—Linux (getrandom → /dev/urandom), Windows (BCryptGenRandom), Darwin (getentropy → /dev/urandom), POSIX fallback (/dev/urandom). Optional CPU RNG (RDRAND/RDSEED) as supplement or fallback only.
- **Rationale**: Matches spec Section 7 and Section 8; getrandom/getentropy preferred where available to avoid keeping a fd open; /dev/urandom (not /dev/random) to avoid blocking.
- **Alternatives considered**: arc4random() (rejected—PRNG, not raw entropy); /dev/random (rejected—can block indefinitely).

## Public API Surface

- **Decision**: Single header `include/truerandom/truerandom.h`; four functions (tr_bytes, tr_uint32, tr_uint64, tr_range) and one public type (tr_error).
- **Rationale**: Minimal API per spec and constitution; no init/teardown; stateless.
- **Alternatives considered**: Multiple headers (rejected—spec mandates single header); handle-based API (rejected—not required for v1).

## Error Model

- **Decision**: Enum `tr_error` with TR_OK, TR_ERR_PARAM, TR_ERR_NOT_SUPPORTED, TR_ERR_IO, TR_ERR_NO_ENTROPY; every failing path returns an error; no partial success.
- **Rationale**: Explicit errors and fail-closed behavior per spec and constitution; errno mapped to tr_error where appropriate.
- **Alternatives considered**: Integer error codes only (rejected—enum is clearer); partial success (rejected—spec forbids).

## Internal Backend Contract

- **Decision**: Internal function contract: fill(buf, len) → success or error; backend writes exactly `len` bytes or returns error without partial fill; no allocation by backend.
- **Rationale**: Layering rule in spec; core.c validates and dispatches; platform code only behind this contract.
- **Alternatives considered**: Per-backend different signatures (rejected—single contract simplifies core).

## Testing Strategy

- **Decision**: Unit tests for null buffer, zero length, invalid range, and (where feasible) mocked backend failure; integration tests on each platform that call API and assert buffer not constant.
- **Rationale**: Constitution test discipline; no entropy distribution tests in unit suite (optional in integration/quality).
- **Alternatives considered**: Entropy quality in unit tests (rejected—flaky and platform-dependent); no integration tests (rejected—spec requires per-platform sanity).

## Dependencies

- **Decision**: Zero mandatory third-party dependencies; only C standard library and platform APIs (and optional CPU RNG instructions).
- **Rationale**: Spec and constitution; reduces integration cost and attack surface.
- **Alternatives considered**: Optional OpenSSL or other crypto lib (rejected for core path—spec forbids mandatory dependency).

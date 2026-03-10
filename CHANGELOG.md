# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [1.0.0] - 2026-03-10

### Added

- Core entropy API: `tr_bytes` fills a caller-provided buffer with OS/hardware entropy bytes.
- Convenience functions: `tr_uint32`, `tr_uint64`, `tr_range` for single random integers.
- Error reporting via `tr_error` enum: `TR_OK`, `TR_ERR_PARAM`, `TR_ERR_NOT_SUPPORTED`, `TR_ERR_IO`, `TR_ERR_NO_ENTROPY`.
- Linux backend using `getrandom(2)` with `/dev/urandom` fallback.
- Windows backend using `BCryptGenRandom`.
- macOS/BSD backend using `getentropy(2)` with `/dev/urandom` fallback.
- POSIX fallback backend using `/dev/urandom`.
- C23 standard with `[[nodiscard]]` on all public functions.
- CMake build system with `BUILD_SHARED_LIBS`, `TRUERANDOM_TESTS`, and `TRUERANDOM_INSTALL` options.
- Unit and integration test suites for all public API functions.
- CI via GitHub Actions on Linux, Windows, and macOS.
- Version macros: `TRUERANDOM_VERSION_MAJOR`, `TRUERANDOM_VERSION_MINOR`, `TRUERANDOM_VERSION_PATCH`, `TRUERANDOM_VERSION`.
- pkg-config support via `libtruerandom.pc`.

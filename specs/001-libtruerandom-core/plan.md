# Implementation Plan: libtruerandom Core Library

**Branch**: `001-libtruerandom-core` | **Date**: 2025-03-09 | **Spec**: [spec.md](./spec.md)  
**Input**: Feature specification from `specs/001-libtruerandom-core/spec.md`

## Summary

Implement a small, portable C library that provides a single API for obtaining true random (entropy) bytes from OS and hardware sources on Linux, Windows, and macOS. Core primitive: fill a caller-provided buffer with entropy; convenience functions for single uint32/uint64 and value-in-range. No mandatory third-party dependencies; explicit error codes; thread-safe; fail closed (no silent weak fallback). Technical approach: C23, compile-time backend selection per platform, internal backend “fill” contract, CMake build.

## Technical Context

**Language/Version**: C23 (ISO/IEC 9899:2024)  
**Primary Dependencies**: None (C standard library and platform entropy APIs only: getrandom/urandom on Unix, BCryptGenRandom on Windows; optional CPU RNG for supplement/fallback)  
**Storage**: N/A (stateless library; no persistent storage)  
**Testing**: Unit tests (parameter validation, error paths), integration tests (per-platform sanity checks); CTest or Make targets  
**Target Platform**: Linux, Windows, macOS (and other POSIX-like systems via fallback)  
**Project Type**: library  
**Performance Goals**: No hard targets; entropy reads may block briefly; avoid unnecessary copies.  
**Constraints**: Minimal API surface; no init/teardown; no mandatory third-party deps; thread-safe; single public header.  
**Scale/Scope**: Single library, ~5–7 source files (core + one backend per platform + optional cpu_rng), one public header.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **I. Minimal API & Single Responsibility**: Plan uses “fill buffer” as core primitive and thin wrappers only; no crypto/PRNG scope. ✅  
- **II. Explicit Errors & Fail Closed**: All functions return `tr_error`; no silent weak fallback; no partial success. ✅  
- **III. Portability & Zero Mandatory Dependencies**: C23 + platform APIs only; Linux/Windows/macOS via one API; backend at compile time. ✅  
- **IV. Test Discipline**: Unit tests for params and errors; integration tests per platform with sanity check; no entropy-quality assertions in unit tests. ✅  
- **V. Versioning & API Stability**: Semver; public API stable in 1.x. ✅  
- **Technical Constraints**: Single header `truerandom/truerandom.h`; naming `tr_`/`TR_`; build system with build/install and optional tests. ✅  
- **Layering**: Platform code only via internal fill contract; no platform types in public API. ✅  

*Post–Phase 1*: No change; design adheres to constitution.

## Project Structure

### Documentation (this feature)

```text
specs/001-libtruerandom-core/
├── plan.md              # This file
├── research.md          # Phase 0 output
├── data-model.md        # Phase 1 output
├── quickstart.md        # Phase 1 output
├── contracts/           # Phase 1 output (public API contract)
└── tasks.md             # Phase 2 output (/speckit.tasks — not created by /speckit.plan)
```

### Source Code (repository root)

```text
include/
└── truerandom/
    └── truerandom.h

src/
├── core.c
├── linux.c
├── windows.c
├── darwin.c
├── posix_fallback.c
├── cpu_rng.c            # optional (RDRAND/RDSEED)
└── sources.h            # internal backend contract

tests/
├── unit/
└── integration/

build/                   # build artifacts (not committed)
```

**Structure Decision**: Single-project C library layout per docs/SPECIFICATION.md. Public API in `include/truerandom/`; core and platform backends in `src/`; tests in `tests/` with unit (param/error) and integration (per-platform) subdirs. Build directory (e.g. `build/`) for out-of-tree artifacts.

## Complexity Tracking

No constitution violations; this section is intentionally empty.

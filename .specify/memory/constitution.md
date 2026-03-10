<!--
Sync Impact Report
==================
Version change: (none) → 1.0.0
Modified principles: N/A (initial fill from template)
Added sections: Technical Constraints, Development Workflow (replaced generic placeholders)
Removed sections: None
Templates requiring updates:
  - .specify/templates/plan-template.md: ✅ Constitution Check references "constitution file" — generic, no update needed
  - .specify/templates/spec-template.md: ✅ No constitution-specific sections; scope aligns (requirements, success criteria)
  - .specify/templates/tasks-template.md: ✅ Task categorization supports testing, setup, phases — no change needed
Follow-up TODOs: None. All placeholders replaced.
-->

# libtruerandom Constitution

## Core Principles

### I. Minimal API & Single Responsibility

The library MUST expose a small, focused API. The core primitive is filling a caller-provided buffer with entropy bytes; convenience functions (`tr_uint32`, `tr_uint64`, `tr_range`) are thin wrappers only. No init/teardown or handle-based API is required for the primary use case. The library's purpose is strictly to supply entropy from OS/hardware sources—not cryptographic primitives, not a full CSPRNG, not network entropy. Any new public API MUST preserve this scope; feature creep into general-purpose crypto or PRNG implementation is out of scope.

**Rationale:** Single responsibility keeps the library portable, auditable, and easy to depend on without pulling in unnecessary code or trust boundaries.

### II. Explicit Errors & Fail Closed

Every library call that can fail MUST return a value of type `tr_error`. There MUST be no silent failure: no returning success with uninitialized data, and no undocumented fallback to weak or non-entropy randomness. On unsupported platform or when no entropy source is available, the library MUST fail with a clear error (e.g. `TR_ERR_NOT_SUPPORTED`, `TR_ERR_NO_ENTROPY`) rather than degrading silently. Partial success is forbidden: either the full requested byte count is written and `TR_OK` is returned, or an error is returned and buffer contents are unspecified. Each public function's possible error returns MUST be documented.

**Rationale:** Security and correctness depend on callers being able to detect and handle failure; silent fallbacks would violate the contract of "true random" and undermine trust.

### III. Portability & Zero Mandatory Dependencies

The implementation MUST depend only on C23 (ISO/IEC 9899:2024) and the target platform's native APIs for entropy (and, where used, CPU RNG instructions). There MUST be no mandatory dependency on third-party libraries (e.g. OpenSSL) for the core path. Optional dependencies, if any, MUST be clearly specified and documented. Support MUST be provided for Linux, Windows, and macOS/BSD via a single API; backend selection is at compile time via standard preprocessor macros (e.g. `_WIN32`, `__linux__`, `__APPLE__`).

**Rationale:** Portability and minimal dependency surface reduce integration cost and attack surface; applications can rely on the library without pulling in large or conflicting stacks.

### IV. Test Discipline

Unit tests MUST cover parameter validation (e.g. null buffer when `len > 0`, zero length, invalid range in `tr_range`) and error paths (e.g. backend failure). Integration tests MUST run on each supported platform, call the public API, assert success, and perform at least a minimal sanity check (e.g. buffer not filled with a single constant byte). Unit tests MUST NOT assert on "entropy quality" (e.g. distribution tests); such checks are optional and belong in integration or quality suites. CI MUST build the library on supported platforms and run the test suite. Static analysis (e.g. Clang static analyzer) is recommended where practical.

**Rationale:** Tests guarantee contract compliance and platform coverage without requiring non-portable or flaky entropy-quality assertions in the core suite.

### V. Versioning & API Stability

The project MUST use semantic versioning (MAJOR.MINOR.PATCH). Within the 1.x series, the public API MUST remain stable: new functions may be added in MINOR versions, but existing functions MUST NOT be removed or changed in a breaking way. Breaking changes require a new MAJOR version. Shared library builds SHOULD preserve ABI compatibility within a MAJOR version. Deprecation of a symbol MUST be announced in the header and release notes; a deprecated symbol MUST remain available for at least one MINOR version before removal in a MAJOR version.

**Rationale:** Predictable versioning and stability allow downstream consumers to upgrade safely and reason about compatibility.

## Technical Constraints

- **Language & standard:** C23 (ISO/IEC 9899:2024). Build MUST set the compiler to C23 (e.g. `-std=c23`).
- **Naming:** All public identifiers use the prefix `tr_`; constants and error codes use the prefix `TR_` (e.g. `TR_OK`, `TR_ERR_IO`). Library name is **libtruerandom**.
- **Public API surface:** Single public header, `include/truerandom/truerandom.h`, consumable as `#include <truerandom/truerandom.h>`. No platform-specific types or backend details in the public API.
- **Backend selection:** At compile time only; no runtime switch for backend. Platform macros (e.g. `_WIN32`, `__linux__`, `__APPLE__`) determine which backend is built.
- **Thread safety:** All public API functions MUST be thread-safe; the implementation MUST NOT rely on unprotected global state for the core entropy path.
- **Build:** Primary build system is CMake or Make; options MUST include at least build/install and optional tests. Build artifacts MUST NOT be committed.

## Development Workflow

- **Specification as source of truth:** All design, API, platform behavior, and file layout are defined in `docs/SPECIFICATION.md`. Implementation and plans MUST align with the spec; any divergence MUST be documented and justified.
- **Constitution Check:** Implementation plans (e.g. from `/speckit.plan`) MUST include a Constitution Check that verifies the plan complies with these principles and technical constraints before Phase 0 research and again after Phase 1 design.
- **Layering:** Platform-specific code MUST be invoked only through the internal source abstraction (the backend "fill" contract). The public API MUST NOT expose platform-specific types, file descriptors, or handles.
- **Review:** Changes that affect the public API, error semantics, or supported platforms MUST be reviewed for spec and constitution compliance.

## Governance

This constitution supersedes ad-hoc development practices for libtruerandom. All work on the library MUST comply with the principles and constraints above. Amendments to the constitution require:

1. Documentation of the proposed change and rationale.
2. Update of the constitution file and version bump (MAJOR for backward-incompatible governance or principle removals; MINOR for new principles or materially expanded guidance; PATCH for clarifications and non-semantic refinements).
3. Propagation of any changes to dependent templates or guidance (e.g. plan-template Constitution Check, spec-template scope) and a sync impact note in the constitution file.

PRs and reviews MUST verify compliance with the constitution where applicable. Complexity or exceptions (e.g. new dependencies, new platforms) MUST be justified against these principles. For detailed API, platform, and build requirements, see `docs/SPECIFICATION.md`.

**Version**: 1.0.0 | **Ratified**: 2025-03-09 | **Last Amended**: 2025-03-09

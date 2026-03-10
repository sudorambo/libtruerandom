# Tasks: libtruerandom Core Library

**Input**: Design documents from `specs/001-libtruerandom-core/`  
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/

**Tests**: Unit and integration tests are required per constitution and plan (parameter validation, error paths, per-platform sanity).

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (US1, US2, US3)
- Include exact file paths in descriptions

## Path Conventions

- **Single project**: `include/`, `src/`, `tests/` at repository root (per plan.md)

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and basic structure

- [x] T001 Create directory structure: include/truerandom/, src/, tests/unit/, tests/integration/ at repository root
- [x] T002 Add root CMakeLists.txt with C23, libtruerandom library target, install (header + library), optional TRUERANDOM_TESTS
- [x] T003 [P] Update .gitignore with build/ and common C build artifacts (e.g. *.o, *.a, *.so)

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core API, internal contract, and one working backend so the library builds and tr_bytes works on at least one platform

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

- [x] T004 Implement public header include/truerandom/truerandom.h (include guard, stddef.h stdint.h, tr_error enum, tr_bytes tr_uint32 tr_uint64 tr_range declarations, extern "C"; document in header which tr_error values each function can return — per constitution II)
- [x] T005 Implement internal backend contract in src/sources.h (fill function declaration/type used by core)
- [x] T006 [P] Implement Linux backend in src/linux.c (getrandom preferred, fallback /dev/urandom)
- [x] T007 Implement core in src/core.c: parameter validation, tr_bytes implementation, compile-time backend dispatch (select linux.c on __linux__)
- [x] T008 Implement tr_uint32, tr_uint64, tr_range in src/core.c as thin wrappers calling internal fill
- [x] T009 [P] Add Windows backend in src/windows.c: full BCryptGenRandom implementation or stub returning TR_ERR_NOT_SUPPORTED (T020 replaces stub if used)
- [x] T010 [P] Add Darwin backend in src/darwin.c: full getentropy/dev/urandom implementation or stub (T021 replaces stub if used)
- [x] T011 [P] Add POSIX fallback backend in src/posix_fallback.c: full /dev/urandom implementation or stub (T022 replaces stub if used)

**Thread safety (FR-008)**: Core and backends MUST be thread-safe; no unprotected global state in the entropy path. Implement accordingly in T007 and backend tasks.

**Checkpoint**: Library builds; tr_bytes and convenience functions work on Linux (and on other platforms if backends implemented). User story implementation can begin.

---

## Phase 3: User Story 1 - Obtain raw entropy bytes (Priority: P1) 🎯 MVP

**Goal**: Callers can fill a buffer with entropy bytes and get a clear success/failure result; tests verify behavior.

**Independent Test**: Call tr_bytes with a fixed-size buffer, assert success and buffer is not a single constant byte; repeat to confirm bytes vary.

### Tests for User Story 1

- [x] T012 [P] [US1] Unit test tr_bytes in tests/unit/test_tr_bytes.c: null buffer with len>0 returns TR_ERR_PARAM, zero length returns TR_OK
- [x] T013 [P] [US1] Integration test tr_bytes in tests/integration/test_tr_bytes.c: call tr_bytes(buf, N), assert TR_OK and buffer not filled with single constant byte

### Implementation for User Story 1

- [x] T014 [US1] Wire unit and integration tests into CMake/CTest in CMakeLists.txt (tests/unit/, tests/integration/)

**Checkpoint**: User Story 1 is fully functional and testable independently (tr_bytes works and is tested).

---

## Phase 4: User Story 2 - Obtain single random integers (Priority: P2)

**Goal**: Callers can obtain a single random uint32/uint64 or value in range with one call; tests verify behavior.

**Independent Test**: Call tr_uint32, tr_uint64, tr_range; assert success and (for range) value in [min, max] inclusive.

### Tests for User Story 2

- [x] T015 [P] [US2] Unit test tr_uint32/tr_uint64/tr_range in tests/unit/test_tr_integers.c: null out returns TR_ERR_PARAM, min>max for tr_range returns TR_ERR_PARAM
- [x] T016 [P] [US2] Integration test single integers in tests/integration/test_tr_integers.c: call tr_uint32/tr_uint64/tr_range, assert TR_OK and (for tr_range) value in [min, max]

### Implementation for User Story 2

- [x] T017 [US2] Add test targets for tests/unit/test_tr_integers.c and tests/integration/test_tr_integers.c in CMakeLists.txt

**Checkpoint**: User Stories 1 and 2 work independently (tr_bytes and convenience APIs tested).

---

## Phase 5: User Story 3 - Predictable error handling (Priority: P3)

**Goal**: Callers can distinguish unsupported platform, I/O failure, and invalid arguments via documented error codes; all platforms supported where applicable.

**Independent Test**: Trigger documented failure modes (null buffer, invalid range, unsupported platform or mocked I/O failure) and assert correct tr_error return.

### Tests for User Story 3

- [x] T018 [P] [US3] Unit test error paths in tests/unit/test_tr_errors.c: TR_ERR_PARAM (null buffer, invalid range), TR_ERR_NOT_SUPPORTED (e.g. on unsupported build); TR_ERR_IO/TR_ERR_NO_ENTROPY only if test harness can simulate or skip (e.g. conditional or mocked)
- [x] T019 [US3] Integration test error codes in tests/integration/test_tr_errors.c: verify TR_ERR_PARAM, TR_ERR_NOT_SUPPORTED (e.g. on unsupported build), and document I/O/entropy behavior

### Implementation for User Story 3

- [x] T020 [US3] If T009 used a stub: implement full Windows backend in src/windows.c (BCryptGenRandom). Ensure core in src/core.c dispatches on _WIN32.
- [x] T021 [US3] If T010 used a stub: implement full Darwin backend in src/darwin.c (getentropy/dev/urandom). Ensure core dispatches on __APPLE__.
- [x] T022 [US3] If T011 used a stub: implement full POSIX fallback in src/posix_fallback.c (/dev/urandom). Ensure core dispatches for other POSIX.
- [x] T023 [US3] Add test targets for tests/unit/test_tr_errors.c and tests/integration/test_tr_errors.c in CMakeLists.txt

**Checkpoint**: All user stories are independently functional; error handling is tested; library supports Linux, Windows, macOS (and POSIX fallback).

---

## Phase 6: Polish & Cross-Cutting Concerns

**Purpose**: Documentation, optional features, and validation

- [x] T024 [P] Add README (or README.md) at repository root: purpose, build/install from quickstart.md, minimal example, link to docs/SPECIFICATION.md
- [ ] T025 [P] Optional: implement src/cpu_rng.c (RDRAND/RDSEED) for supplement/fallback per spec; integrate in core if enabled — **deferred to post-1.0**
- [x] T026 Add LICENSE file at repository root with chosen license terms — **verified: LICENSE exists**
- [x] T027 Run quickstart.md validation: build, install, compile and run minimal example from quickstart
- [x] T028 Add CI (e.g. GitHub Actions or similar) that builds the library and runs the test suite on supported platforms (Linux, Windows, macOS or documented subset) — per constitution IV

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies — can start immediately
- **Foundational (Phase 2)**: Depends on Setup — BLOCKS all user stories
- **User Stories (Phase 3–5)**: Depend on Foundational
  - US1 (Phase 3): After Foundational; no dependency on US2/US3
  - US2 (Phase 4): After Foundational; depends on US1 implementation (same core, add tests)
  - US3 (Phase 5): After Foundational; completes backends and error tests
- **Polish (Phase 6)**: Depends on completion of desired user stories. T028 (CI) satisfies constitution IV (build and test on supported platforms).

### User Story Dependencies

- **User Story 1 (P1)**: After Foundational — tests for tr_bytes
- **User Story 2 (P2)**: After Foundational — tests for tr_uint32/tr_uint64/tr_range (implementation in Foundational)
- **User Story 3 (P3)**: After Foundational — error-path tests and remaining backends

### Within Each User Story

- Tests can be added in parallel where different files
- Implementation tasks (e.g. wire tests in CMake) follow test file creation
- Story complete when tests pass and independent test criteria from spec are met

### Parallel Opportunities

- T003, T006, T009, T010, T011 can run in parallel within their phase
- T012, T013; T015, T016; T018 can run in parallel within their phase
- T024, T025 can run in parallel in Polish; T028 (CI) can follow T027 or run in parallel with documentation tasks

---

## Parallel Example: User Story 1

```bash
# Unit and integration tests for tr_bytes (after Foundational):
Task T012: "Unit test tr_bytes in tests/unit/test_tr_bytes.c"
Task T013: "Integration test tr_bytes in tests/integration/test_tr_bytes.c"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup  
2. Complete Phase 2: Foundational (CRITICAL)  
3. Complete Phase 3: User Story 1 (tests for tr_bytes)  
4. **STOP and VALIDATE**: Run unit and integration tests for tr_bytes  
5. Library is usable for “fill buffer with entropy” on supported platform(s)

### Incremental Delivery

1. Setup + Foundational → library builds, API works on at least one platform  
2. Add User Story 1 tests → tr_bytes validated (MVP)  
3. Add User Story 2 tests → convenience APIs validated  
4. Add User Story 3 tests and backends → all platforms and error codes validated  
5. Polish → README, LICENSE, optional cpu_rng, quickstart validation

### Parallel Team Strategy

- One developer: complete phases in order (Setup → Foundational → US1 → US2 → US3 → Polish)  
- Multiple developers: after Foundational, one can own test files (T012–T013, T015–T016, T018–T019, T023) while another completes backends (T020–T022) and CMake test wiring (T014, T017, T023)

---

## Notes

- [P] tasks = different files, no dependencies on other tasks in same phase  
- [USn] label maps task to user story for traceability  
- Each user story phase is independently testable per spec “Independent Test”  
- Commit after each task or logical group  
- Backend strategy: T009–T011 either implement full backends or stubs returning TR_ERR_NOT_SUPPORTED. T020–T022: if stubs were used, replace with full implementation and ensure core dispatches; if full backends were done in T009–T011, only verify core dispatch.

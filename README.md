# libtruerandom

A small, portable C library that provides **true random (entropy)** bytes from operating-system and hardware sources. One API for Linux, Windows, and macOS—no platform-specific code in your application, no mandatory third-party dependencies.

## Purpose

- **Fill a buffer** with entropy bytes (`tr_bytes`) for nonces, tokens, session IDs, or seeding a PRNG.
- **Single integers** via `tr_uint32`, `tr_uint64`, or `tr_range(min, max)`.
- **Explicit errors**: `TR_OK`, `TR_ERR_PARAM`, `TR_ERR_NOT_SUPPORTED`, `TR_ERR_IO`, `TR_ERR_NO_ENTROPY`—no silent fallback to weak randomness.

See [docs/SPECIFICATION.md](docs/SPECIFICATION.md) for the full specification.

## Build and install

From the repository root:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install . --prefix /usr/local
```

Options:

- `-DTRUERANDOM_TESTS=ON` — build and register tests; then run `ctest` from the build directory.
- `-DBUILD_SHARED_LIBS=OFF` — build static library instead of shared.

More detail: [specs/001-libtruerandom-core/quickstart.md](specs/001-libtruerandom-core/quickstart.md).

## Minimal example

```c
#include <truerandom/truerandom.h>
#include <stdio.h>

int main(void) {
    unsigned char buf[16];
    tr_error err = tr_bytes(buf, sizeof buf);
    if (err != TR_OK) {
        fprintf(stderr, "tr_bytes failed: %d\n", (int)err);
        return 1;
    }
    for (size_t i = 0; i < sizeof buf; i++)
        printf("%02x", buf[i]);
    printf("\n");
    return 0;
}
```

Link with `-ltruerandom` and ensure the install prefix’s `include` and `lib` are in your include and library paths (e.g. `-I/usr/local/include -L/usr/local/lib`).

## License

See [LICENSE](LICENSE).

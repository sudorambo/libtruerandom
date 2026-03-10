# Quickstart: libtruerandom

**Feature**: 001-libtruerandom-core  
**Date**: 2025-03-09

Minimal steps to build, install, and use the library. Exact commands depend on the build system (CMake or Make) once implemented.

## Build (from repository root)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Optional: enable tests (if option exists):

```bash
cmake .. -DTRUERANDOM_TESTS=ON
cmake --build .
ctest
```

## Install

```bash
# From build directory
cmake --install . --prefix /usr/local
```

Or set `-DCMAKE_INSTALL_PREFIX=/your/prefix` when configuring.

## Use in a consumer project

1. **Include the header**

   ```c
   #include <truerandom/truerandom.h>
   ```

2. **Link the library**

   Link with `-ltruerandom` and ensure the install prefix’s `include` and `lib` are in your include and library paths (e.g. `-I/usr/local/include`, `-L/usr/local/lib`).

3. **Minimal example: fill 16 bytes and print hex**

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

4. **Example: get a single random value in a range**

   ```c
   #include <truerandom/truerandom.h>
   #include <stdio.h>

   int main(void) {
       uint32_t value;
       tr_error err = tr_range(&value, 1, 6);  /* 1..6 inclusive */
       if (err != TR_OK) {
           fprintf(stderr, "tr_range failed\n");
           return 1;
       }
       printf("value: %u\n", (unsigned)value);
       return 0;
   }
   ```

## Error handling

Always check the return value. Example:

```c
tr_error err = tr_bytes(buf, len);
switch (err) {
    case TR_OK:
        break;  /* use buf */
    case TR_ERR_PARAM:
        /* invalid arguments */
        break;
    case TR_ERR_NOT_SUPPORTED:
        /* platform unsupported */
        break;
    case TR_ERR_IO:
        /* I/O error */
        break;
    case TR_ERR_NO_ENTROPY:
        /* no entropy available */
        break;
    default:
        /* handle unknown */
        break;
}
```

## Requirements

- C23-capable compiler (e.g. GCC 14+, Clang 18+, MSVC 2022 17.8+).
- Supported platform: Linux, Windows, or macOS (see docs/SPECIFICATION.md for backend details).
- No mandatory third-party libraries for the core API.

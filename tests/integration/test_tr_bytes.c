/**
 * Integration test for tr_bytes: success and buffer not constant.
 */
#include "../../include/truerandom/truerandom.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fail_count = 0;

static void expect_eq(int line, tr_error got, tr_error want, const char *msg)
{
	if (got != want) {
		printf("FAIL line %d: %s (got %d, want %d)\n", line, msg, (int)got, (int)want);
		fail_count++;
	}
}

#define EXPECT_EQ(got, want, msg) expect_eq(__LINE__, (got), (want), (msg))

static int is_constant(const unsigned char *buf, size_t len)
{
	if (len == 0) return 1;
	unsigned char c = buf[0];
	for (size_t i = 1; i < len; i++)
		if (buf[i] != c) return 0;
	return 1;
}

int main(void)
{
	unsigned char buf[32];
	tr_error err;

	err = tr_bytes(buf, sizeof(buf));
	EXPECT_EQ(err, TR_OK, "tr_bytes(buf, 32)");
	if (err == TR_OK && is_constant(buf, sizeof(buf))) {
		printf("FAIL: buffer is filled with a single constant byte\n");
		fail_count++;
	}

	/* Second call to see variation */
	err = tr_bytes(buf, sizeof(buf));
	EXPECT_EQ(err, TR_OK, "tr_bytes second call");

	if (fail_count != 0) {
		printf("Integration test test_tr_bytes: %d failure(s)\n", fail_count);
		return EXIT_FAILURE;
	}
	printf("Integration test test_tr_bytes: pass\n");
	return EXIT_SUCCESS;
}

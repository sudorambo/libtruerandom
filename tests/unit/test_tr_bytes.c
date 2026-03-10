/**
 * Unit tests for tr_bytes: parameter validation.
 */
#include "../../include/truerandom/truerandom.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int fail_count = 0;

static void expect_eq(int line, tr_error got, tr_error want, const char *msg)
{
	if (got != want) {
		printf("FAIL line %d: %s (got %d, want %d)\n", line, msg, (int)got, (int)want);
		fail_count++;
	}
}

#define EXPECT_EQ(got, want, msg) expect_eq(__LINE__, (got), (want), (msg))

int main(void)
{
	unsigned char buf[16];

	/* null buffer with len > 0 -> TR_ERR_PARAM */
	EXPECT_EQ(tr_bytes(NULL, 1), TR_ERR_PARAM, "tr_bytes(NULL, 1)");
	EXPECT_EQ(tr_bytes(NULL, 16), TR_ERR_PARAM, "tr_bytes(NULL, 16)");

	/* zero length -> TR_OK (buf may be null) */
	EXPECT_EQ(tr_bytes(NULL, 0), TR_OK, "tr_bytes(NULL, 0)");
	EXPECT_EQ(tr_bytes(buf, 0), TR_OK, "tr_bytes(buf, 0)");

	if (fail_count != 0) {
		printf("Unit test test_tr_bytes: %d failure(s)\n", fail_count);
		return EXIT_FAILURE;
	}
	printf("Unit test test_tr_bytes: pass\n");
	return EXIT_SUCCESS;
}

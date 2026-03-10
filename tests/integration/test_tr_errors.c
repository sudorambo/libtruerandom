/**
 * Integration test for error codes: TR_ERR_PARAM, TR_ERR_NOT_SUPPORTED.
 * I/O and no-entropy behavior are platform-dependent; document in test output.
 */
#include "../../include/truerandom/truerandom.h"
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
	unsigned char buf[4];
	uint32_t u32;

	EXPECT_EQ(tr_bytes(NULL, 1), TR_ERR_PARAM, "tr_bytes(NULL,1)");
	EXPECT_EQ(tr_range(&u32, 2, 1), TR_ERR_PARAM, "tr_range min>max");

	/* On supported platform we get TR_OK; on unsupported we'd get TR_ERR_NOT_SUPPORTED.
	 * This test runs on current build target only. */
	tr_error err = tr_bytes(buf, sizeof(buf));
	if (err != TR_OK && err != TR_ERR_NOT_SUPPORTED) {
		printf("Note: tr_bytes returned %d (I/O or no-entropy possible on this platform)\n", (int)err);
	}

	if (fail_count != 0) {
		printf("Integration test test_tr_errors: %d failure(s)\n", fail_count);
		return EXIT_FAILURE;
	}
	printf("Integration test test_tr_errors: pass\n");
	return EXIT_SUCCESS;
}

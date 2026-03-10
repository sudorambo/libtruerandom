/**
 * Unit tests for error paths: TR_ERR_PARAM, TR_ERR_NOT_SUPPORTED where applicable.
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
	uint32_t u32;

	/* Invalid parameters -> TR_ERR_PARAM */
	EXPECT_EQ(tr_bytes(NULL, 8), TR_ERR_PARAM, "tr_bytes(NULL, 8)");
	EXPECT_EQ(tr_uint32(NULL), TR_ERR_PARAM, "tr_uint32(NULL)");
	EXPECT_EQ(tr_range(&u32, 5, 3), TR_ERR_PARAM, "tr_range min>max");

	/* TR_ERR_NOT_SUPPORTED: only on unsupported build; we don't mock here */
	/* TR_ERR_IO / TR_ERR_NO_ENTROPY: would require mocked backend - skip in unit */

	if (fail_count != 0) {
		printf("Unit test test_tr_errors: %d failure(s)\n", fail_count);
		return EXIT_FAILURE;
	}
	printf("Unit test test_tr_errors: pass\n");
	return EXIT_SUCCESS;
}

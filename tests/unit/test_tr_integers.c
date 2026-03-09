/**
 * Unit tests for tr_uint32, tr_uint64, tr_range: null and invalid range.
 */
#include <truerandom/truerandom.h>
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
	uint64_t u64;

	/* null out -> TR_ERR_PARAM */
	EXPECT_EQ(tr_uint32(NULL), TR_ERR_PARAM, "tr_uint32(NULL)");
	EXPECT_EQ(tr_uint64(NULL), TR_ERR_PARAM, "tr_uint64(NULL)");
	EXPECT_EQ(tr_range(NULL, 0, 1), TR_ERR_PARAM, "tr_range(NULL, 0, 1)");

	/* min > max -> TR_ERR_PARAM */
	EXPECT_EQ(tr_range(&u32, 2, 1), TR_ERR_PARAM, "tr_range(&u32, 2, 1)");
	EXPECT_EQ(tr_range(&u32, 10, 5), TR_ERR_PARAM, "tr_range(&u32, 10, 5)");

	if (fail_count != 0) {
		printf("Unit test test_tr_integers: %d failure(s)\n", fail_count);
		return EXIT_FAILURE;
	}
	printf("Unit test test_tr_integers: pass\n");
	return EXIT_SUCCESS;
}

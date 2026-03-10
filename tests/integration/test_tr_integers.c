/**
 * Integration test for tr_uint32, tr_uint64, tr_range: success and range.
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
	uint64_t u64;
	tr_error err;

	err = tr_uint32(&u32);
	EXPECT_EQ(err, TR_OK, "tr_uint32");
	err = tr_uint64(&u64);
	EXPECT_EQ(err, TR_OK, "tr_uint64");

	for (int i = 0; i < 20; i++) {
		uint32_t v;
		err = tr_range(&v, 1, 6);
		EXPECT_EQ(err, TR_OK, "tr_range(1,6)");
		if (err == TR_OK && (v < 1 || v > 6)) {
			printf("FAIL: tr_range returned %u not in [1,6]\n", (unsigned)v);
			fail_count++;
		}
	}

	if (fail_count != 0) {
		printf("Integration test test_tr_integers: %d failure(s)\n", fail_count);
		return EXIT_FAILURE;
	}
	printf("Integration test test_tr_integers: pass\n");
	return EXIT_SUCCESS;
}

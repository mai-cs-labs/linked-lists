#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <stdio.h>

#define TEST(NAME) static void NAME(bool* test__result)

#define TEST_ASSERT(EXPR) do { \
    if (!(EXPR)) { \
        if (*test__result) \
            fprintf(stderr, "#%d: %s failed:\n", test__total, __FUNCTION__); \
        fprintf(stderr, "\t%s:%d: %s\n", __FILE__, __LINE__, #EXPR); \
        *test__result = false; \
    } \
} while (0)

typedef void (*test_fn)(bool* test__result);

extern int test__passed;
extern int test__total;

extern int test_run(const test_fn tests[]);

#ifdef TEST_IMPLEMENTATION

int test__total = 0;
int test__passed = 0;

int test_run(const test_fn tests[])
{
    for (const test_fn* t = tests; *t != NULL; ++t) {
        bool result = true;
        ++test__total;

        (*t)(&result);

        if (result == true)
            ++test__passed;
    }

    if (test__passed != test__total) {
        fprintf(stderr, "\n○ %d/%d tests passed\n", test__passed, test__total);
        return 1;
    } else {
        fprintf(stderr, "\n◉ All tests passed!\n");
        return 0;
    }
}

#endif // TEST_IMPLEMENTATION
#endif // __TEST_H__

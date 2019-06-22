
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "RingBuffer.h"

#define MAX_TEXT 1000

#define ASSERT_TRUE(expr, result) \
    if (result.success && !(expr)) { \
        result.success = false; \
        result.line = __LINE__; \
        strncpy(result.e, #expr, MAX_TEXT); \
    }

typedef struct test_result_t {
    bool            success;
    int             line;
    char            e[MAX_TEXT];
} test_result_t;

typedef struct test_definition_t {
    const char *name;
    test_result_t (*function)();
} test_definition_t;

test_result_t test_empty(void);
test_result_t test_basic(void);
test_result_t test_wrap(void);
test_result_t test_full(void);
test_result_t test_fill_consume_all_refill(void);
test_result_t test_fill_consume_part_refill(void);
test_result_t test_lag(void);

static test_definition_t all_tests[] = {
        {"Empty", &test_empty},
        {"Basic", &test_basic},
        {"Wrap", &test_wrap},
        {"Full", &test_full},
        {"Fill, consume all and refill", &test_fill_consume_all_refill},
        {"Fill, consume some and refill", &test_fill_consume_part_refill},
        {"Lag", &test_lag}
};

void
init_result(test_result_t *result)
{
    result->success = true;
    result->line = 0;
};

/**
 * Can't rb_get() from an empty buffer
 */
test_result_t
test_empty(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);

    ASSERT_TRUE(stat == RB_OK, result);

    int32_t val = 99;

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(val == 99, result);

    return result;
}

/**
 * Case that doesn't wrap
 */
test_result_t
test_basic(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);

    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 22);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 33);
    ASSERT_TRUE(stat == RB_OK, result);

    int32_t val;

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 22, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 33, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(val == 33, result);


    return result;
}

/**
 * Wrapping works when buffer doesn't fill
 */
test_result_t
test_wrap(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);
    ASSERT_TRUE(stat == RB_OK, result);

    for (int32_t i = 0; i < 20; i++) {
        int32_t input = 1000 + i;
        // printf("put %d\n", i);
        stat = rb_put(buffer, input);
        ASSERT_TRUE(stat == RB_OK, result);
        // if (stat != RB_OK) printf("bad\n");
        int32_t output;
        // printf("get %d\n", i);
        stat = rb_get(buffer, &output);
        // if (stat != RB_OK) printf("bad\n");
        ASSERT_TRUE(stat == RB_OK, result);
        // if (output != input) printf("bad data\n");
        ASSERT_TRUE(output == input, result);
    }

    return result;
}

/**
 * Can fill and empty without wrap
 */
test_result_t
test_full(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);

    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 11);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 22);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 33);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 44);
    ASSERT_TRUE(stat == RB_ERROR, result);

    int32_t val;

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 11, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 22, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 33, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(val == 33, result);

    return result;
}

/**
 * Fill a buffer, consume all and then fill again
 */
test_result_t
test_fill_consume_all_refill(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);

    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 11);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 22);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 33);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 44);
    ASSERT_TRUE(stat == RB_ERROR, result);

    // now full

    int32_t val;

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 11, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 22, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 33, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(val == 33, result);

    // now empty

    stat = rb_put(buffer, 55);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 66);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 77);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 88);
    ASSERT_TRUE(stat == RB_ERROR, result);

    // now full again

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 55, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 66, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 77, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(val == 77, result);

    // now empty again

    return result;

}

/**
 * Fill a buffer, consume some and then fill again
 */
test_result_t
test_fill_consume_part_refill(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);

    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 11);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 22);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 33);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 44);
    ASSERT_TRUE(stat == RB_ERROR, result);

    // now full

    int32_t val;

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 11, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 22, result);

    // now has one left, so put two more

    stat = rb_put(buffer, 55);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 66);
    ASSERT_TRUE(stat == RB_OK, result);

    stat = rb_put(buffer, 77);
    ASSERT_TRUE(stat == RB_ERROR, result);

    // now full again

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 33, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 55, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(val == 66, result);

    stat = rb_get(buffer, &val);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(val == 66, result);

    // now empty again

    return result;
}


/**
 * Always two values in buffer
 * @return
 */
test_result_t
test_lag(void) {
    test_result_t result;
    init_result(&result);

    ring_buffer_t *buffer;

    RB_Status stat = rb_init(&buffer, 3);
    ASSERT_TRUE(stat == RB_OK, result);

    int32_t limit = 20;
    for (int32_t i = 0; i < limit; i++) {
        int32_t input = 1000 + i;
        stat = rb_put(buffer, input);
        ASSERT_TRUE(stat == RB_OK, result);
        if (i > 0) {
            int32_t output;
            int32_t expected = input - 1;
            stat = rb_get(buffer, &output);
            ASSERT_TRUE(stat == RB_OK, result);
            ASSERT_TRUE(output == expected, result);
        }
    }
    int32_t output;
    int32_t expected = 1000 + limit - 1;
    stat = rb_get(buffer, &output);
    ASSERT_TRUE(stat == RB_OK, result);
    ASSERT_TRUE(output == expected, result);

    // now empty
    stat = rb_get(buffer, &output);
    ASSERT_TRUE(stat == RB_ERROR, result);
    ASSERT_TRUE(output == expected, result);

    return result;
}

int
main (int argc, char** argv) {
    size_t test_count = sizeof(all_tests) / sizeof(test_definition_t);
    uint16_t skip_count = 0;
    uint16_t fail_count = 0;

    for (uint8_t i = 0; i < test_count; i++) {
        test_definition_t *test = all_tests + i;
        fprintf(stdout, "[%u] %-40s ", i + 1, test->name);
        test_result_t result = (test->function)();
        fprintf(stdout, "%s \n", result.success ? "PASS" : "FAIL");
        if (!result.success) {
            fprintf(stdout, "........ Line: %d\n", result.line);
            fprintf(stdout, "........ Condition: %s\n", result.e);
            fail_count++;
        }
    }
    if (fail_count == 0) {
        fprintf(stdout, "All %lu tests passed\n", test_count);
    } else {
        fprintf(stdout, "%d of %lu tests failed\n", fail_count, test_count);
    }
    if (skip_count != 0) {
        fprintf(stderr, "%d tests skipped\n", skip_count);
    } else {
        fprintf(stdout, "No tests were skipped\n");
    }

    return 0;
}


#include "mathlib.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

void test_in_range() {
    double num = mathlib_rand_in_range(0, 10);
    assert(num > 0 && num < 10);

    num = mathlib_rand_in_range(10, 100);
    assert(num > 10 && num < 100);

    num = mathlib_rand_in_range(10, 11);
    assert(num > 10 && num < 11);
}

void test_uniform() {
    int nums[10];
    for (size_t i = 0; i < 10; i++) {
        nums[i] = 0;
    }
    
    for (size_t i = 0; i < 10000; i++) {
        double num = mathlib_rand_in_range(0, 10);
        nums[(int) num]++;
    }

    for (size_t i = 0; i < 10; i++) {
        assert(fabs(nums[i] / 10000. - 0.1) < 0.01);
    }
}

void test_min() {
    assert(mathlib_min(6, 8) == 6);
    assert(mathlib_min(9, 3) == 3);
    assert(mathlib_min(7, 7) == 7);
}

void test_max() {
    assert(mathlib_max(6, 8) == 8);
    assert(mathlib_max(9, 3) == 9);
    assert(mathlib_max(7, 7) == 7);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_in_range)
    DO_TEST(test_uniform)
    DO_TEST(test_min)
    DO_TEST(test_max)

    puts("mathlib_test PASS");
}
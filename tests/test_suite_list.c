#include "body.h"
#include "test_util.h"
#include <assert.h>
#include <stdlib.h>

const rgb_color_t WHITE_C = {.r = 1, .g = 1, .b = 1};
const double MASS = 1;
const size_t LARGE_SIZE = 1000;


void size_zero_list() {
    list_t *empty = list_init(0, (free_func_t)body_free);
    assert(list_size(empty) == 0);
    list_free(empty);
}

void access_empty_list(void *list) {
    list_get((list_t *)list, 0);
}
void size_one_list() {
    list_t *body_list = list_init(1, (free_func_t)body_free);
    list_t *shape = list_init(0, free);
    body_t *b1 = body_init(shape, MASS, WHITE_C);
    
    list_add(body_list, b1);
    assert(list_size(body_list) == 1);
    assert(list_get(body_list, 0) == b1);

    list_remove_first(body_list);
    assert(list_size(body_list) == 0);
    assert(test_assert_fail(access_empty_list, (void *)body_list));

    list_free(body_list);
}

void out_of_bounds1(void *list) {
    list_t *body_list = (list_t *)list;
    list_get(body_list, list_size(body_list));
}
void out_of_bounds2(void *list) {
    list_t *body_list = (list_t *)list;
    list_get(body_list, list_size(body_list) + 50);
}
void large_list() {
    list_t *body_list = list_init(1, (free_func_t)body_free);
    body_t **body_array = malloc(sizeof(body_t *) * LARGE_SIZE); 

    for (size_t i = 0; i < LARGE_SIZE; i++) {
        list_t *l = list_init(0, free);
        body_t *b = body_init(l, MASS, WHITE_C);
        body_array[i] = b;
        list_add(body_list, b);
    }

    assert(list_size(body_list) == LARGE_SIZE);

    for (size_t i = 0; i < LARGE_SIZE; i++) {
        assert(list_get(body_list, i) == body_array[i]);
    }

    assert(test_assert_fail(out_of_bounds1, (void *)body_list));
    assert(test_assert_fail(out_of_bounds2, (void *)body_list));

    for (size_t i = 0; i < LARGE_SIZE; i++) {
        list_remove_first(body_list);
        assert(list_size(body_list) == LARGE_SIZE - i - 1);
    }

    list_free(body_list);
    free(body_array);
}



int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(size_zero_list)
    DO_TEST(size_one_list)
    DO_TEST(large_list)

    puts("list_test PASS");
}
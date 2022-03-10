#include "shape.h"
#include "test_util.h"
#include <assert.h>
#include <stdlib.h>

const int N_POINTS = 5;
const double DENSITY = 1;
const bool RAND_SIZE = false;
const bool INIT_VELO = true;

void test_shape_is_on_screen() {
    vector_t lower_bound = {.x = 0, .y = 0};
    vector_t upper_bound = {.x = 1000, .y = 500};

    // test star at the center of the screen
    vector_t center = {.x = 500, .y = 250};
    body_t *star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    assert(body_is_on_screen(star, lower_bound, upper_bound) == true);
    body_free(star);
    
    //test star at the right edge of the screen
    center = (vector_t) {.x = 1000, .y = 250};
    star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    assert(body_is_on_screen(star, lower_bound, upper_bound) == true);
    body_free(star);
    
    //test star at the top edge of the screen
    center = (vector_t) {.x = 500, .y = 500};
    star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    assert(body_is_on_screen(star, lower_bound, upper_bound) == true);
    body_free(star);
    
    //test star barely off the screen
    center = (vector_t) {.x = 1051, .y = 500};
    star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    assert(body_is_on_screen(star, lower_bound, upper_bound) == false);
    body_free(star);
    
    //test star barely off the screen
    center = (vector_t) {.x = 500, .y = 551};
    star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    assert(body_is_on_screen(star, lower_bound, upper_bound) == false);
    body_free(star);
    
    //test star completely off the screen
    center = (vector_t) {.x = 1500, .y = 1000};
    star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    assert(body_is_on_screen(star, lower_bound, upper_bound) == false);
    body_free(star);
}

void test_rotate_shape() {
    vector_t center = {.x = 200, .y = 50};

    body_t *star = shape_init_random_star(N_POINTS, DENSITY, center, RAND_SIZE, INIT_VELO);
    double dt = 1;

    list_t *polygon = body_get_shape_nocpy(star);

    // Store copies of all the vertices of the polygon
    vector_t vert_arr[10];
    for (int i = 0; i < list_size(polygon); i++) {
        vert_arr[i] = *(vector_t *)list_get(polygon, i);
    }

    body_set_rotation(star, body_get_rotation(star) + shape_get_rot_speed() * dt);

    // check if each vertex is rotated by the right amount
    for (int i = 0; i < list_size(polygon); i++) {
        vector_t old_vertex = vert_arr[i];

        // apply a series of transformations to rotate the vertex about the center
        vector_t rotated_old_vertex = vec_subtract(old_vertex, center);
        rotated_old_vertex = vec_rotate(rotated_old_vertex, shape_get_rot_speed() * dt);
        rotated_old_vertex = vec_add(rotated_old_vertex, center);

        assert(vec_isclose(rotated_old_vertex, *(vector_t *)list_get(polygon, i)));
    }

    body_free(star);
}

void test_shape_update() {
    vector_t start_position = {.x = 1000, .y = 500};
    vector_t lower_bound = {.x = 0, .y = 0};
    vector_t upper_bound = {.x = 1000, .y = 500};
    vector_t net_acceleration = VEC_ZERO;
    double min_elasticity = 1;
    double max_elasticity = 1;
    body_t *star = star = shape_init_random_star(N_POINTS, DENSITY, start_position, RAND_SIZE, INIT_VELO);
    double dt = 1;
    
    // test collision with the wall
    // need to update three times to see if it bounces off the wall and comes back
    for (int i = 0; i < 3; i++) {
        shape_update(star, dt, lower_bound, upper_bound, net_acceleration,
                     min_elasticity, max_elasticity);
    }
    vector_t centroid = body_get_centroid(star);
    // centroid should be to the left and below the star_position
    assert(centroid.x < start_position.x);
    assert(centroid.y < start_position.y);
    body_free(star);

    // test velocity with no acceleration
    start_position = (vector_t) {.x = 500, .y = 250};
    star = shape_init_random_star(N_POINTS, DENSITY, start_position, RAND_SIZE, INIT_VELO);
    shape_update(star, dt, lower_bound, upper_bound, net_acceleration,
                 min_elasticity, max_elasticity);
    centroid = body_get_centroid(star);
    vector_t disp = vec_multiply(dt, body_get_velocity(star));
    vector_t expected_position = vec_add(start_position, disp);
    assert(vec_isclose(expected_position, centroid));
    body_free(star);

    // test velocity with acceleration
    net_acceleration = (vector_t) {.x = 20, .y = 20};
    star = shape_init_random_star(N_POINTS, DENSITY, start_position, RAND_SIZE, INIT_VELO);
    shape_update(star, dt, lower_bound, upper_bound, net_acceleration,
                 min_elasticity, max_elasticity);
    centroid = body_get_centroid(star);
    disp = vec_multiply(dt, body_get_velocity(star));
    expected_position = vec_add(start_position, disp);
    assert(vec_isclose(expected_position, centroid));
    // test again
    shape_update(star, dt, lower_bound, upper_bound, net_acceleration,
                min_elasticity, max_elasticity);   
    centroid = body_get_centroid(star);
    disp = vec_add(disp, vec_multiply(dt, body_get_velocity(star)));
    expected_position = vec_add(start_position, disp);
    assert(vec_isclose(expected_position, centroid));

    body_free(star);
}


int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_shape_is_on_screen)
    DO_TEST(test_rotate_shape)
    DO_TEST(test_shape_update)

    puts("shape_test PASS");
}
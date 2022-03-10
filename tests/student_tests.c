#include "forces.h"
#include "body.h"
#include "scene.h"
#include "vector.h"
#include "test_util.h"
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>


list_t *make_shape() {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){-1, +1};
    list_add(shape, v);
    return shape;
}


typedef struct const_force_aux {
    vector_t F;
    body_t *body;
} const_force_aux_t;


void force_creator_const(const_force_aux_t *aux) {
    assert(aux);

    body_t *b = aux->body;

    assert(b);

    body_add_force(b, aux->F);
}


// Test that a body with constant external force and drag reaches a correct terminal velocity
void test_terminal_velocity() {
    const double M = 10;
    const vector_t F = {.x = 0, .y = -10};
    const double GAMMA = 500;
    const double DT = 1e-6;
    const size_t STEPS = 1000000;

    scene_t *scene = scene_init(VEC_ZERO);
    body_t *mass = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    scene_add_body(scene, mass);

    const_force_aux_t *aux = malloc(sizeof(const_force_aux_t));
    aux->body = mass;
    aux->F = F;

    scene_add_force_creator(scene, (force_creator_t)force_creator_const, aux, free);

    create_drag(scene, GAMMA, mass);

    vector_t terminal_velocity = vec_multiply(1. / GAMMA, F);
    double old_diff = INFINITY;
    for (size_t i = 0; i < STEPS; i++) {
        vector_t v = body_get_velocity(mass);
        assert(fabs(v.x) <= fabs(terminal_velocity.x));
        assert(fabs(v.y) <= fabs(terminal_velocity.y));
        double diff = vec_distance(v, terminal_velocity);
        assert(diff <= old_diff);
        old_diff = diff;
        scene_tick(scene, DT);
    }
    assert(vec_isclose(body_get_velocity(mass), terminal_velocity));

    scene_free(scene);
}


void test_gravity() {
    const double M1 = 10, M2 = 1e9;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000;

    scene_t *scene = scene_init(VEC_ZERO);
    body_t *body_one = body_init(make_shape(), M1, (rgb_color_t) {0.5, 0.5, 0.5});
    body_t *body_two = body_init(make_shape(), M2, (rgb_color_t) {0.2, 0.2, 0.2});

    body_set_centroid(body_one, (vector_t) {1000, 250});
    body_set_centroid(body_two, (vector_t) {500, 250});

    scene_add_body(scene, body_one);
    scene_add_body(scene, body_two);
    create_newtonian_gravity(scene, G, body_one, body_two);

    vector_t velo = body_get_velocity(body_one);
    vector_t old_centroid = body_get_centroid(body_one);
    for(int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);
        assert(within(1e-4, body_get_centroid(body_two).x, 500));
        double new_pos_x = (old_centroid.x + velo.x * DT);
        bool left = new_pos_x < 500;
        if(left) {
            assert(body_get_velocity(body_one).x > velo.x);
        }
        if(!left) {
            assert(body_get_velocity(body_one).x < velo.x);
        }
        velo = body_get_velocity(body_one);
        old_centroid = body_get_centroid(body_one);
    }
    scene_free(scene);
}


void test_drag() {
    const double M1 = 100;
    const double GAMMA = 20;
    const int STEPS = 100000;
    const double DT = 1e-6;

    vector_t centroid = {.x = 100, .y = 500};
    vector_t velo = {.x = 200, .y = 0};
    scene_t *scene = scene_init(VEC_ZERO);
    body_t *body = body_init(make_shape(), M1, (rgb_color_t){0.2, 0.2, 0.2});
    body_set_centroid(body, centroid);
    body_set_velocity(body, velo);
    scene_add_body(scene, body);
    create_drag(scene, GAMMA, body);
    
    double time = 0;
    for (int i = 0; i < STEPS; i++) {
        time = DT * i;
        assert(within(1e-4, body_get_velocity(body).x, velo.x * pow(M_E, -GAMMA * time / M1)));
        // assert(vec_isclose(body_get_velocity(body),(vector_t) {velo.x * pow(M_E, -GAMMA * time / M1), 0}));
        scene_tick(scene, DT);
    }
    scene_free(scene);
}


int main(int argc, char *argv[]) {
     // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_terminal_velocity)
    DO_TEST(test_gravity)
    DO_TEST(test_drag)

    puts("forces_test PASS");
}


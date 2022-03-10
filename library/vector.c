#include "vector.h"
#include "mathlib.h"
#include <math.h>


const vector_t VEC_ZERO = {.x = 0, .y = 0};


vector_t vec_add(vector_t v1, vector_t v2) {
    vector_t new_vec;
    new_vec.x = v1.x + v2.x;
    new_vec.y = v1.y + v2.y;

    return new_vec;
}


vector_t vec_subtract(vector_t v1, vector_t v2) {
    vector_t new_vec;
    new_vec.x = v1.x - v2.x;
    new_vec.y = v1.y - v2.y;

    return new_vec;
}


vector_t vec_negate(vector_t v) {
    // -v = 0 - v
    return vec_subtract(VEC_ZERO, v);
}


vector_t vec_multiply(double scalar, vector_t v) {
    vector_t new_vec;
    new_vec.x = v.x * scalar;
    new_vec.y = v.y * scalar;

    return new_vec;
}


double vec_dot(vector_t v1, vector_t v2) {
    double x_cont = v1.x * v2.x;
    double y_cont = v1.y * v2.y;
    
    return x_cont + y_cont;
}


double vec_cross(vector_t v1, vector_t v2) {
    return v1.x * v2.y - v1.y * v2.x;
}


vector_t vec_rotate(vector_t v, double angle) {
    vector_t new_vec;
    new_vec.x = v.x * cos(angle) - v.y * sin(angle);
    new_vec.y = v.x * sin(angle) + v.y * cos(angle);

    return new_vec;
}


double vec_distance(vector_t p1, vector_t p2) {
    return vec_magnitude(vec_subtract(p1, p2));
}


double vec_magnitude(vector_t v) {
    return sqrt(v.x * v.x + v.y * v.y);
}


vector_t vec_unit(vector_t v) {
    return vec_multiply(1 / vec_magnitude(v), v);
}
#include "polygon.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


double const POLYGON_INF_VAL = 10000;


double polygon_signed_area(list_t *polygon) {
    size_t n = list_size(polygon);
    double area = 0;

    // Bounds of sums in formula are 1-indexed [1, n-1] so our 0-indexed bounds
    // are [0, n-1)
    for (size_t i = 0; i < n; i++) {
        // x_i and y_i
        vector_t *v1 = list_get(polygon, i);
        // x_{i+1} and y_{i+1}
        size_t v2_idx = (i + 1) % n;
        vector_t *v2 = list_get(polygon, v2_idx);

        // Add first sum contribution (positive coefficient sum)
        area += v1->x * v2->y;

        // Add second sum contribution (negative coefficient sum)
        area -= v1->y * v2->x;
    }

    // area is equal to 1/2 the absolute value of the sum of terms
    area /= 2;

    return area;
}


double polygon_area(list_t *polygon) {
    // Get the signed area and take the absolute value
    double signed_area = polygon_signed_area(polygon);
    if (signed_area < 0) {
        signed_area *= -1;
    }
    return signed_area;
}


vector_t polygon_centroid(list_t *polygon) {
    size_t n = list_size(polygon);
    double c_x = 0;
    double c_y = 0;
    for (size_t i = 0; i < n; i++) {
        // v_i
        vector_t *v1 = list_get(polygon, i);
        // v_{i+1}
        size_t v2_idx = (i + 1) % n;
        vector_t *v2 = list_get(polygon, v2_idx);

        // Add contributions to centroid
        c_x += (v1->x + v2->x) * vec_cross(*v1, *v2);
        c_y += (v1->y + v2->y) * vec_cross(*v1, *v2);
    }

    // Divide centroid coordinates by 6 * the signed area
    double signed_area = polygon_signed_area(polygon);
    c_x /= 6 * signed_area;
    c_y /= 6 * signed_area;

    vector_t centroid = {.x = c_x, .y = c_y};

    return centroid;
}


void polygon_translate(list_t *polygon, vector_t translation) {
    for (size_t i = 0; i < list_size(polygon); i++) {
        vector_t *v = list_get(polygon, i);
        *v = vec_add(*v, translation);
    }
}


void polygon_rotate(list_t *polygon, double angle, vector_t point) {
    // Translate all vertices so that point is the origin
    polygon_translate(polygon, vec_negate(point));
    // Rotate about the point
    for (size_t i = 0; i < list_size(polygon); i++) {
        vector_t *v = list_get(polygon, i);
        *v = vec_rotate(*v, angle);
    }
    // Return to (0, 0) origin
    polygon_translate(polygon, point);
}
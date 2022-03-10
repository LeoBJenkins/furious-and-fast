#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "polygon.h"
#include "mathlib.h"
#include "shape.h"


// Defining properties of the desired star
const double OUTER_RADIUS_BASE = 50;
const double INNER_RADIUS_BASE = 20;
const double SCALE_MIN = 0.6;
const double SCALE_MAX = 2;

// Defining properties of the desired circle
// Not a perfect circle. It has to approximated using many edges.
const size_t EDGES = 30;

// Defining properties of the shape
const vector_t DEFAULT_VELOCITY = {.x = 50, .y = 50};
const double DEFAULT_ROT_SPEED = M_PI / 4;


body_t *shape_init_star(size_t n_points, double outer_radius,
                        double inner_radius, double density,
                        vector_t velocity, vector_t center,
                        rgb_color_t color) {

    size_t n_vertices = n_points * 2;
    list_t *polygon = list_init(n_vertices, free);
    double rot_per_point = -2 * M_PI / n_points;

    // Top point of the star. Center is not included so this can be rotated
    // about the center of the star.
    vector_t top = {.x = 0, .y = outer_radius};

    // Find the coordinates of the first inner vertex
    double rot_from_top = -M_PI / n_points;
    vector_t inner_top = {.x = 0, .y = inner_radius};
    // First inner vertex of the star. Center is not included so this can be
    // rotated about the center of the star.
    vector_t inner_start = vec_rotate(inner_top, rot_from_top);

    // Add the vertices of the star to the polygon by rotating the initial
    // vertices found above
    for (size_t i = 0; i < n_points; i++) {
        double rotation = rot_per_point * i;
        // Add the outer vertex by rotating the from the top of the star
        vector_t *v = malloc(sizeof(vector_t));
        assert(v);
        *v = vec_rotate(top, rotation);
        *v = vec_add(*v, center);
        list_add(polygon, v);

        // Add the inner vertex by rotating from the first inner vertex
        v = malloc(sizeof(vector_t));
        assert(v);
        *v = vec_rotate(inner_start, rotation);
        *v = vec_add(*v, center);
        list_add(polygon, v);
    }

    double mass = density * polygon_area(polygon);

    body_t *star = body_init(polygon, mass, color);
    body_set_velocity(star, velocity);

    return star;
}


body_t *shape_init_random_star(size_t n_points, double density, vector_t center,
                               bool rand_size, bool init_velocity) {
    // Randomly generate properties for the star
    double scale = rand_size ? mathlib_rand_in_range(SCALE_MIN, SCALE_MAX) : 1;
    double outer_radius = OUTER_RADIUS_BASE * scale;
    double inner_radius = INNER_RADIUS_BASE * scale;
    double r = mathlib_rand_in_range(0, 1);
    double g = mathlib_rand_in_range(0, 1);
    double b = mathlib_rand_in_range(0, 1);
    rgb_color_t color = {.r = r, .g = g, .b = b};
    vector_t velocity = init_velocity ? DEFAULT_VELOCITY : VEC_ZERO;

    return shape_init_star(n_points, outer_radius, inner_radius, density,
                           velocity, center, color);
}


body_t *shape_init_circle_sector(double radius, double sector_angle,
                                 rgb_color_t color, double density,
                                 void *info, free_func_t info_freer) {
    return shape_init_circle_sector_with_sprite(radius, sector_angle, color, density, 
                                                info, info_freer, NULL, VEC_ZERO);
}


body_t *shape_init_circle_sector_with_sprite(double radius, double sector_angle, rgb_color_t color,
                                             double density, void *info, free_func_t info_freer,
                                             const char *filename, vector_t dimensions) {
    list_t *points = list_init(EDGES, free);
    vector_t *pen = malloc(sizeof(vector_t));
    assert(pen);

    // if it is not a fill circle, start point at the origin
    if (sector_angle != 0) {
        *pen = VEC_ZERO;
        list_add(points, pen);
    }

    pen = malloc(sizeof(vector_t));
    assert(pen);
    pen->x = radius * cos(sector_angle / 2);        
    pen->y = radius * sin(sector_angle / 2);
    list_add(points, pen);

    double rot_angle = (2 * M_PI - sector_angle) / EDGES;
    for (size_t i = 0; i < (EDGES - 1); i++) {
        vector_t *new_pen = malloc(sizeof(vector_t));
        assert(new_pen);

        *new_pen = vec_rotate(*pen, rot_angle);
        list_add(points, new_pen);
        pen = new_pen;
    }

    double mass = density * polygon_area(points);

    if (filename) {
        return body_init_with_info_and_sprite(points, mass, color, info,
                                              info_freer, filename, dimensions);
    }
    return body_init_with_info(points, mass, color, info, info_freer);                               
}


body_t *shape_init_circle(double radius, rgb_color_t color, double density,
                          void *info, free_func_t info_freer) {
    return shape_init_circle_sector(radius, 0, color, density, info, info_freer);
}


body_t *shape_init_circle_with_sprite(double radius, rgb_color_t color, double density,
                                      void *info, free_func_t info_freer, const char *filename,
                                      vector_t dimensions) {
    return shape_init_circle_sector_with_sprite(radius, 0, color, density, info,
                                                info_freer, filename, dimensions);
}


body_t *shape_init_oval(double a, double b, size_t num_edges,
                        rgb_color_t color, double density,
                        void *info, free_func_t info_freer) {
    
    list_t *oval_shape = list_init(num_edges, free);

    double dTheta = 2. * M_PI / num_edges;
    for (size_t i = 0; i < num_edges; i++) {
        // https://math.stackexchange.com/questions/475436/2d-point-projection-on-an-ellipse/
        double theta = dTheta * i;
        double k = a * b / sqrt(b * b * cos(theta) * cos(theta) 
                                + a * a * sin(theta) * sin(theta));
        double x = k * cos(theta);
        double y = k * sin(theta);
        vector_t *new_point = malloc(sizeof(vector_t));
        assert(new_point);
        *new_point = (vector_t){.x = x, .y = y};
        list_add(oval_shape, new_point);
    }

    double oval_mass = M_PI * a * b * density;
    return body_init_with_info(oval_shape, oval_mass, color, info, info_freer);
}


body_t *shape_init_rectangle(double l, double h, 
                             rgb_color_t color, double density,
                             void *info, free_func_t info_freer) {
    return shape_init_rectangle_with_sprite(l, h, color, density, info, info_freer, NULL, VEC_ZERO);
}


body_t *shape_init_rectangle_with_sprite(double l, double h, rgb_color_t color, double density,
                                         void *info, free_func_t info_freer, const char *filename,
                                         vector_t dimensions) {
    list_t *rectangle_points = list_init(4, free);
    vector_t *bot_left = malloc(sizeof(vector_t));
    assert(bot_left);
    *bot_left = (vector_t) {.x = (-l/2.), .y = (-h/2.)};
    list_add(rectangle_points, bot_left);

    vector_t *top_left = malloc(sizeof(vector_t));
    assert(top_left);
    *top_left = (vector_t) {.x = (-l/2.), .y = (h/2.)};
    list_add(rectangle_points, top_left);
       
    vector_t *top_right = malloc(sizeof(vector_t));
    assert(top_right);
    *top_right = (vector_t) {.x = (l/2.), .y = (h/2.)};
    list_add(rectangle_points, top_right);
        
    vector_t *bot_right = malloc(sizeof(vector_t));
    assert(bot_right);
    *bot_right = (vector_t) {.x = (l/2.), .y = (-h/2.)};
    list_add(rectangle_points, bot_right);
        
    double rect_mass = density * l * h;
        
    if (filename) {
        return body_init_with_info_and_sprite(rectangle_points, rect_mass, color,
                                                  info, info_freer, filename, dimensions);
    }
    return body_init_with_info(rectangle_points, rect_mass, color, info, info_freer);
}


body_t *shape_init_needle(double radius, double length, rgb_color_t color, void *info, free_func_t info_freer) {
    list_t *points = list_init(20, free);

    vector_t *pen = malloc(sizeof(vector_t));
    assert(pen);

    pen->x = 0;
    pen->y = length;
    list_add(points, pen);

    vec_rotate(*pen, M_PI / 2);

    double rot_angle = (M_PI) / (EDGES / 2);
    for (size_t i = 0; i < (EDGES / 2); i++) {
        vector_t *new_pen = malloc(sizeof(vector_t));
        assert(new_pen);

        *new_pen = vec_rotate(*pen, rot_angle);
        list_add(points, new_pen);
        pen = new_pen;
    }

    double mass = 1;
    return body_init_with_info(points, mass, color, info, info_freer);
}


body_t *shape_init_triangle_with_info(double width, double height, rgb_color_t color, double mass, void *info, free_func_t info_freer) {
    list_t *points = list_init(3, free);

    vector_t *point1 = malloc(sizeof(vector_t));
    point1->x = -1.0 * (width / 2.0);
    point1->y = 0;
    list_add(points, point1);
    vector_t *point2 = malloc(sizeof(vector_t));
    point2->x = (width / 2.0);
    point2->y = 0;
    list_add(points, point2);
    vector_t *point3 = malloc(sizeof(vector_t));
    point3->x = 0;
    point3->y = height;
    list_add(points, point3);

    return body_init_with_info(points, mass, color, info, info_freer);
}


body_t *shape_init_ai_collider(body_t *ai_car) {
    vector_t dimensions = body_get_dimensions(ai_car);
    rgb_color_t black = {.r = 0, .g = 0, .b = 0};
    body_t *ai_collider = shape_init_triangle_with_info(4 * dimensions.x, 20, black, 0.1, (void*)ai_car, NULL);
    return ai_collider;
}


body_t *shape_init_player_indicator(body_t *player_car) {
    vector_t dimensions = body_get_dimensions(player_car);
    rgb_color_t blue = {.r = 0, .g = 0, .b = 0.8};
    body_t *indicator = shape_init_triangle_with_info(0.5 * dimensions.x, 20, blue, 0.1, (void*)player_car, NULL);
    return indicator;
}


// Performs the motion of the shape over a time period dt, handling collisions
void shape_update(body_t *shape, double dt, vector_t lower_bounds, 
                  vector_t upper_bounds, vector_t net_acceleration,
                  double min_elasticity, double max_elasticity) {

    assert(shape);

    vector_t dv = vec_multiply(dt, net_acceleration);
    body_set_velocity(shape, vec_add(body_get_velocity(shape), dv));

    body_tick(shape, dt);

    body_set_rotation(shape, body_get_rotation(shape) + DEFAULT_ROT_SPEED * dt);

    list_t *polygon = body_get_shape_nocpy(shape);
    vector_t shape_v = body_get_velocity(shape);
    for (size_t i = 0; i < list_size(polygon); i++) {
        vector_t *p = list_get(polygon, i);
        bool hit_wall = false;
        // Check for x collisions
        if ((p->x <= lower_bounds.x && shape_v.x < 0)
            || (p->x >= upper_bounds.x && shape_v.x > 0)) {
            double elasticity = mathlib_rand_in_range(min_elasticity, 
                                                      max_elasticity);
            shape_v.x *= -elasticity;
            body_set_velocity(shape, shape_v);
            hit_wall = true;
        }
        // Check for y collisions
        if ((p->y <= lower_bounds.y && shape_v.y < 0)
            || (p->y >= upper_bounds.y && shape_v.y > 0)) {
            double elasticity = mathlib_rand_in_range(min_elasticity, 
                                                      max_elasticity);
            shape_v.y *= -elasticity;
            body_set_velocity(shape, shape_v);
            hit_wall = true;
        }
        // If we hit a wall, break (either no need to continue or we could
        // reverse velocity again accidentally!)
        if (hit_wall) {
            break;
        }
    }
}


double shape_get_rot_speed() {
    return DEFAULT_ROT_SPEED;
}
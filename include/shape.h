#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "body.h"

/**
 * Initializes and returns a star with the specified characteristics
 * 
 * @param n_points the number of points of the star
 * @param outer_radius the outer radius of the star
 * @param inner_radius the inner radius of the star
 * @param density the density of the star
 * @param velocity the velocity of the star
 * @param center the center of the star
 * @param color the color of the star
 * @return a pointer to the initialized star
 */
body_t *shape_init_star(size_t n_points, double outer_radius,
                        double inner_radius, double density,
                        vector_t velocity, vector_t center,
                        rgb_color_t color);

/**
 * Initializes and returns a star with random characteristics
 * 
 * @param n_points the number of points of the star
 * @param density the density of the star
 * @param center the center of the star
 * @param rand_size argument for whether the star should have a random size
 * @param init_velocity argument for whether the star should have an inital velocity
 * Starts with DEFAULT_VELOCITY = {.x = 50, .y = 50} if true
 * @return a pointer to the randomly initialized star
 */
body_t *shape_init_random_star(size_t n_points, double density, vector_t center,
                               bool rand_size, bool init_velocity);

/**
 * Initializes and returns a circle with a slice cut out.
 * Asserts that the required memory is allocated.
 * 
 * @param radius the radius of the circle
 * @param sector_angle the angle of the slice that is cut out
 * @param color the color of the circle
 * @param density the density of the circle
 * @param info the info for the circle
 * @param info_freer free function for the info
 * @return a pointer to the initialized circle
 */ 
body_t *shape_init_circle_sector(double radius, double sector_angle,
                                 rgb_color_t color, double density,
                                 void *info, free_func_t info_freer);

/**
 * Initializes and returns a circle with a slice cut out with a sprite.
 * Asserts that the required memory is allocated.
 * 
 * @param radius the radius of the circle
 * @param sector_angle the angle of the slice that is cut out
 * @param color the color of the circle
 * @param density the density of the circle
 * @param info the info for the circle
 * @param info_freer free function for the info
 * @param filename filename of the sprite image
 * @param dimensions dimensions of the sprite image
 * @return a pointer to the initialized circle
 */ 
body_t *shape_init_circle_sector_with_sprite(double radius, double sector_angle, rgb_color_t color,
                                             double density, void *info, free_func_t info_freer,
                                             const char *filename, vector_t dimensions);

/**
 * Initializes and returns a full circle
 * Asserts that the required memory is allocated.
 * 
 * @param radius the radius of the circle
 * @param color the color of the circle
 * @param density the density of the circle
 * @param info the info for the circle
 * @param info_freer free function for the info
 * @return a pointer to the initialized circle
 */ 
body_t *shape_init_circle(double radius, rgb_color_t color, double density,
                          void *info, free_func_t info_freer);

/**
 * Initializes and returns a full circle with a sprite
 * Asserts that the required memory is allocated.
 * 
 * @param radius the radius of the circle
 * @param color the color of the circle
 * @param density the density of the circle
 * @param info the info for the circle
 * @param info_freer free function for the info
 * @param filename filename of the sprite image
 * @param dimensions dimensions of the sprite image
 * @return a pointer to the initialized circle
 */ 
body_t *shape_init_circle_with_sprite(double radius, rgb_color_t color, double density,
                                      void *info, free_func_t info_freer, const char *filename,
                                      vector_t dimensions);

/**
 * Initializes and returns an oval body.
 *
 * @param a the radius of the major axis
 * @param b the radius of the minor axis
 * @param num_edges the number of edges to use to estimate the oval
 * @param color the desired color of the oval
 * @param density the density of the oval
 * @param info the info for the oval
 * @param info_freer free function for the info
 * @return a pointer to the initialized oval
 */
body_t *shape_init_oval(double a, double b, size_t num_edges,
                        rgb_color_t color, double density,
                        void *info, free_func_t info_freer);


/**
 * Initializes and returns an rectangle body.
 *
 * @param l the length
 * @param h the height
 * @param color the desired color of the rectangle
 * @param density the density of the rectangle
 * @param info the info for the rectangle
 * @param info_freer free function for the info
 * @return a pointer to the initialized rectangle
 */
body_t *shape_init_rectangle(double l, double h, 
                        rgb_color_t color, double density,
                        void *info, free_func_t info_freer);

/**
 * Initializes and returns an rectangle body with a sprite.
 *
 * @param l the length
 * @param h the height
 * @param color the desired color of the rectangle
 * @param density the density of the rectangle
 * @param info the info for the rectangle
 * @param info_freer free function for the info
 * @param filename filename of the sprite image
 * @param dimensions dimensions of the sprite image
 * @return a pointer to the initialized rectangle
 */
body_t *shape_init_rectangle_with_sprite(double l, double h, rgb_color_t color, double density,
                                         void *info, free_func_t info_freer, const char *filename,
                                         vector_t dimensions);

/**
 * Initializes and returns a needle.
 *
 * @param radius the radius
 * @param length the length
 * @param color the desired color of the needle
 * @param density the density of the needle
 * @param info the info for the needle
 * @param info_freer free function for the info
 * @return a pointer to the initialized needle
 */
body_t *shape_init_needle(double radius, double length, rgb_color_t color, void *info,
                          free_func_t info_freer);

body_t *shape_init_ai_collider(body_t *ai_car);

body_t *shape_init_player_indicator(body_t *player_car);

/**
 * Updates the shape after some time has elapsed
 * 
 * @param star a pointer to the shape to be updated
 * @param lower_bound the lower bound of the window
 * @param upper_bound the upper bound of the window
 * @param net_acceleration the net acceleration of the shape
 * @param min_elasticity the minimum elasticity when bouncing off a wall
 * @param max_elasticity the maximum elasticity when bouncing off a wall
 */
void shape_update(body_t *shape, double dt, vector_t lower_bounds, 
                  vector_t upper_bounds, vector_t net_acceleration,
                  double min_elasticity, double max_elasticity);

/**
 * Gets the shape's rotation speed
 * 
 * @return the shape's rotation speed
 */
double shape_get_rot_speed();

#endif // #ifndef __SHAPE_H__
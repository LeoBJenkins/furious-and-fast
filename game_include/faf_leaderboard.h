#ifndef __FAF_LEADERBOARD_H__
#define __FAF_LEADERBOARD_H__

#include "faf_levels.h"
#include "hud.h"
#include <stdbool.h>

// How many records are stored for a given level
extern const size_t FAF_NUM_RECORDS;

// Struct for a single record
typedef struct faf_record faf_record_t;

// Struct for an entire leaderboard for a level
typedef struct faf_leaderboard faf_leaderboard_t;

typedef struct faf_lb_hud {
    size_t idx;
    widget_t *times[5];
} faf_lb_hud_t;

char *faf_get_level_fname(faf_level_t level);

void faf_save_leaderboard(faf_leaderboard_t *leaderboard);

faf_leaderboard_t *faf_get_leaderboard(faf_level_t level);

bool faf_update_leaderboard(faf_level_t level, double time);

void faf_leaderboard_hud_update(hud_t *hud);


#endif // #ifndef __FAF_LEADERBOARD_H__
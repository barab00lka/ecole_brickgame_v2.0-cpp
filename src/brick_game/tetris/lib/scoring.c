#include <scoring.h>

#include <stdio.h>
#include <string.h>

// updates score based on how many lines cleared
void score_update(stats_t *stats, int lines_cleared) {
  unsigned old_score = stats->score;

  switch (lines_cleared) {
    case 0:
      return;
    case 1:
      stats->score += 100;
      break;
    case 2:
      stats->score += 300;
      break;
    case 3:
      stats->score += 700;
      break;
    default:
      stats->score += 1500;
      break;
  }

  if (stats->level < TETRIS_MAX_LVL) {
    stats->level += (stats->score / 600) - (old_score / 600);
    if (stats->level > TETRIS_MAX_LVL) stats->level = TETRIS_MAX_LVL;
  }
}

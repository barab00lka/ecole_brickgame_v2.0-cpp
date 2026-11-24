#ifndef DEFINES_H_
#define DEFINES_H_

#ifndef FIELD_ROWS
#define FIELD_ROWS (20)
#endif
#ifndef FIELD_COLS
#define FIELD_COLS (10)
#endif

#define WALL_RIGHT FIELD_ROWS
#define WALL_LEFT -1

#define SNAKE_TICKRATE (10)
#define SNAKE_ANIM_TICKRATE (5)
#define SNAKE_SPEED_STEP (1)
#define SNAKE_MAX_LVL (10)
#define SNAKE_SAVE_FN "./snake_save.bin"

#define TETRIS_SAVE_FN "./tetris_save.bin"
#define TETRIS_SPEED_STEP (4)
#define TETRIS_MAX_LVL (10)

#define TETRIS_ANIM_TICKRATE (5)
#define TETRIS_TICKRATE (48)
#define TETRAMINO_ORIENTATIONS (4)
#define TETRAMINO_SIZE_MAX (4)

#endif

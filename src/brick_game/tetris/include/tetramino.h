#ifndef TETRAMINO_H_
#define TETRAMINO_H_

#include <stdbool.h>

#include <brickgame_api.h>
#include <entities.h>

piece_t get_random_piece();
bool can_spawn_piece(piece_t *p, const unsigned *grid);
void rotate_piece_cw(piece_t *p, const unsigned *grid);
void steer_piece(UserAction_t sig, piece_t *p, const unsigned *grid);
void attach_piece(const piece_t *p, unsigned *grid);
bool try_drop_down(piece_t *p, const unsigned *grid);

// clang-format off
static const unsigned short shapes[shapeCount][TETRAMINO_ORIENTATIONS] = {
    [tShape] = {
	0b0000001001110000,
	0b0000010001100100,
	0b0000000001110010,
	0b0000000100110001,
    }, 
    [gShape] = {
	0b0000010001110000,
	0b0000011001000100,
	0b0000000001110001,
	0b0000000100010011,
    },
    [lShape] = {
	0b0000000101110000,
	0b0000010001000110,
	0b0000000001110100,
	0b0000001100010001,
    },
    [squareShape] = {
        0b0000011001100000,
        0b0000011001100000,
        0b0000011001100000,
        0b0000011001100000,
    },
    [stickShape] = {
        0b0000111100000000,
        0b0010001000100010,
        0b0000111100000000,
        0b0010001000100010,
    },
    [rzShape] = {
        0b0000001101100000,
        0b0010001100010000,
        0b0000001101100000,
        0b0010001100010000,
    }, 
    [zShape] = {
        0b0000011000110000,
        0b0001001100100000,
        0b0000011000110000,
        0b0001001100100000,
    },
};
// clang-format on
#endif

#ifndef FIELD_H_
#define FIELD_H_

#include <defines.h>
#include <tetramino.h>

#define FULL_LINE_MARK 1U << 31
#define SET_MARK(x) (x |= FULL_LINE_MARK)
#define REMOVE_MARK(x) (x |= ~FULL_LINE_MARK)
#define COMPLETE_LINE ((1U << FIELD_COLS) - 1)
#define IS_FULL_LINE(x) ((x & COMPLETE_LINE) == COMPLETE_LINE)
#define IS_MARKED(x) (FULL_LINE_MARK & x)

/* сдвигаем битовую маску самого поля в центр числа, чтобы отлавливать биты
 * залезшие за левую границу при проверке столкновений 00000000000 | 1111111111
 * | 00000000000 */
#define MARGIN 11U

int complete_linecount(unsigned *grid);
int clearing_animation(unsigned *field);
void shift_remove_lines(unsigned *field);
void clear_field(unsigned *field);
bool check_collision(const piece_t *p, const unsigned *grid);

#endif

#ifndef VIEW_H_
#define VIEW_H_

#include <stdbool.h>

#include <entities.h>

#ifdef __cplusplus
extern "C" {
#endif

// the int** matrix has to be preallocated by the caller
void plot_field(tetrisCtx_t *ctx, int **v);
// the int** matrix has to be preallocated by the caller
void plot_next(tetrisCtx_t *ctx, int **v);
// initializes static 2D arrays for grid and next piece window

#ifdef __cplusplus
}
#endif
#endif

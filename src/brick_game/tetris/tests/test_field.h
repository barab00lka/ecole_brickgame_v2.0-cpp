#ifndef TEST_FIELD_H_
#define TEST_FIELD_H_

#include <check.h>
#include <string.h>

#include "../include/field.h"
#include "../include/tetramino.h"

void print_field_bin(uint32_t* grid);
Suite* field_suite(void);
#endif

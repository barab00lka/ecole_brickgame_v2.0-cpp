#include <check.h>
#include <stdlib.h>

#include "test_field.h"
#include "test_fsm.h"
#include "test_scoring.h"
#include "test_tetramino.h"

int main(void) {
  int failed;
  SRunner *sr = srunner_create(NULL);
  srunner_add_suite(sr, field_suite());
  srunner_add_suite(sr, tetramino_suite());
  srunner_add_suite(sr, scoring_suite());
  srunner_add_suite(sr, fsm_suite());
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

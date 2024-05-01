#include "../../brick_game/tetris/tetris.h"
#include "test.h"


START_TEST(game) {
    inf_loop();
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);

  tcase_add_test(tc1_1, game);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
//   nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}

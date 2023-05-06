#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>
#include <string.h>

#include "../src/controller.h"

// NOLINTBEGIN(*-magic-numbers)

Test(process_input, reg_input) {
  // check that a regular non-special card input returns the same string
  char buf[3];
  strcpy(buf, "B9");
  char processed_input[3];
  process_input(buf, processed_input);
  cr_assert(eq(str, processed_input, buf));
}

Test(process_input, rev_input) {
  // check that a reverse card input returns the color + '10' to indicate that
  // it's a reverse card
  char buf[3];
  strcpy(buf, "Rr");
  char processed_input[4];
  process_input(buf, processed_input);
  cr_assert(eq(str, processed_input, "R10"));
}

Test(process_input, skip_input) {
  // check that a skip card input returns the color + '11' to indicate that it's
  // a skip card
  char buf[3];
  strcpy(buf, "YS");
  char processed_input[4];
  process_input(buf, processed_input);
  cr_assert(eq(str, processed_input, "Y11"));
}

Test(process_input, draw_2_input) {
  // check that a draw 2 card returns the color + '12' to indicate that it's a
  // draw 2 card
  char buf[3];
  strcpy(buf, "G2");
  char processed_input[4];
  process_input(buf, processed_input);
  cr_assert(eq(str, processed_input, "G12"));
}

Test(process_input, wild_input) {
  // check that a wild card input returns the color chose + '13' to indicate
  // that it's a wild card
  char buf[3];
  strcpy(buf, "WR");
  char processed_input[4];
  process_input(buf, processed_input);
  cr_assert(eq(str, processed_input, "R13"));
}

Test(process_input, wild_4_input) {
  // check that a draw 4 wild card input returns the color chosen + '14' to
  // indicate that it's a draw 4 wild card
  char buf[4];
  strcpy(buf, "W4Y");
  char processed_input[4];
  process_input(buf, processed_input);
  cr_assert(eq(str, processed_input, "Y14"));
}

// NOLINTEND(*-magic-numbers)

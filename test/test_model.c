#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>

#include "../src/model.h"

// NOLINTBEGIN(*-magic-numbers)

// When making a card, the color and value should be set correctly.
Test(make_card, value_set_correctly) {
  card* card_ = make_card('r', 1);
  cr_assert(eq(str, card_->color, 'r'));
  cr_assert(eq(sz, card_->value, 1));
  // free_node(node_);
}

// Making a new card should set the next card to the null pointer by default.
Test(make_card, next_is_null) {
  card* card_ = make_card('r', 1);
  cr_assert(eq(ptr, card_->next, NULL));
  free_card(card_);
}

// There aren't good, reliable ways to check free_card other than Valgrind, so
// don't add any unit tests for free_card.

// Making a new deck should create an empty deck, so the head should be the null
// pointer.
Test(make_deck, null_head) {
  deck* deck_ = make_deck();
  cr_assert(zero(ptr, deck_->head));
  free_deck(deck_);
}

// Making a new deck should create an empty deck, so the size should be 0.
Test(make_deck, zero_len) {
  deck* deck_ = make_deck();
  cr_assert(zero(sz, deck_->size));
  free_deck(deck_);
}
// Tests that if the card is the head it properly moves.
Test(move_card, card_head) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);
  append_card(deck_1, 'Y', 7);
  append_card(deck_1, 'G', 7);

  deck* deck_2 = make_deck();
  append_card(deck_2, 'R', 7);
  move_card(deck_1->head, deck_1, deck_2);
  cr_assert(eq(sz, deck_1, 2));
  cr_assert(eq(str, deck_1->head->color, "Y"));
  free_deck(deck_1);
  free_deck(deck_2);
}

// Tests that if the card is the head and its next is null it properly moves.
Test(move_card, card_null) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);

  deck* deck_2 = make_deck();
  append_card(deck_2, 'R', 7);
  move_card(deck_1->head, deck_1, deck_2);
  move_card(deck_1->head, deck_1, deck_2);
  move_card(deck_1->head, deck_1, deck_2);
  cr_assert(zero(sz, deck_1->size));
  cr_assert(eq(str, deck_1->head->color, "Y"));
  free_deck(deck_1);
  free_deck(deck_2);
}
// Tests that if the card is in the middle of the list it properly moves.
Test(move_card, card_middle) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);
  append_card(deck_1, 'Y', 7);
  append_card(deck_1, 'G', 7);

  deck* deck_2 = make_deck();
  append_card(deck_2, 'R', 7);
  move_card(deck_1->head->next, deck_1, deck_2);
  cr_assert(eq(sz, deck_1, 2));
  cr_assert(eq(str, deck_1->head->color, "B"));
  free_deck(deck_1);
  free_deck(deck_2);
}
// Tests that if the second list is empty it properly moves.
Test(move_card, empty_list) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);
  append_card(deck_1, 'Y', 7);
  append_card(deck_1, 'G', 7);

  deck* deck_2 = make_deck();
  move_card(deck_1->head, deck_1, deck_2);
  cr_assert(eq(sz, deck_1, 2));
  cr_assert(eq(str, deck_1->head->color, "Y"));
  free_deck(deck_1);
  free_deck(deck_2);
}
// Tests that if the first list is one and the second list is empty it moves
// correctly.
Test(move_card, empty_second) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);

  deck* deck_2 = make_deck();
  move_card(deck_1->head, deck_1, deck_2);
  cr_assert(zero(sz, deck_1->size));
  cr_assert(eq(str, deck_2->head->color, "B"));
  free_deck(deck_1);
  free_deck(deck_2);
}

// Appending to a list should increase the length by one.
Test(append_card, increment_length) {
  deck* deck_ = make_deck();
  cr_assert(zero(sz, deck_->size));
  append_card(deck_, 1, 'R');
  cr_assert(eq(sz, deck_->size, 1));
  free_deck(deck_);
}

// Appending to a list should increase the length by one.
Test(append_card, append_empty) {
  deck* deck_ = make_deck();
  append_card(deck_, 1, 'R');
  // cr_assert(eq(sz, deck_->size, 1));
  cr_assert(eq(str, deck_->head->color, 'R'));
  // cr_assert(eq(ptr, final->next, NULL));
  free_deck(deck_);
}

// Appending to a deck should add the correct value at the correct index.
Test(append_card, correct_value_added) {
  deck* setup_deck = make_deck();
  append_card(setup_deck, 1, 'R');
  append_card(setup_deck, 2, 'W');
  append_card(setup_deck, 3, 'B');
  append_card(setup_deck, 4, 'B');
  card* final = get_card_index(setup_deck, 3);
  cr_assert(eq(sz, final->value, 4));
  cr_assert(eq(str, final->color, 'B'));
  cr_assert(eq(ptr, final->next, NULL));
  free_deck(setup_deck);
}


// NOLINTEND(*-magic-numbers)

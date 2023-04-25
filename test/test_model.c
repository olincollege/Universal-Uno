#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>

#include "../src/model.h"

// NOLINTBEGIN(*-magic-numbers)

// When making a card, the color and value should be set correctly.
Test(make_card, value_set_correctly) {
  card* card_ = make_card('r', 1);
  cr_assert(eq(char, card_->ch, 'r'));
  cr_assert(eq(size_t, card_->val, 1));
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
  free_list(deck_);
}

// Making a new deck should create an empty deck, so the size should be 0.
Test(make_deck, zero_len) {
  deck* deck_ = make_deck();
  cr_assert(zero(sz, deck_->size));
  free_deck(deck_);
}

// NOLINTEND(*-magic-numbers)

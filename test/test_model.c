#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>

#include "../src/model.h"

// NOLINTBEGIN(*-magic-numbers)

// When making a card, the color and value should be set correctly.
Test(make_card, color_set_correctly) {
  card* card_ = make_card('R', 1);
  cr_expect(eq(chr, card_->color, 'R'), "Wrong color! Expected %c, got %c", 'R',
              card_->color);
  free_card(card_);
}

Test(make_card, value_set_correctly) {
  card* card_ = make_card('R', 1);
   cr_expect(eq(sz, card_->value, 1), "Wrong value! Expected %zu, got %zu", 1,
            card_->value);
   free_card(card_);
}
Test(make_card, values_set_correctly) {
  card* card_ = make_card('W', 14);
   cr_expect(eq(sz, card_->value, 14), "Wrong value! Expected %zu, got %zu", 14,
            card_->value);
  cr_expect(eq(chr, card_->color, 'W'));
   free_card(card_);
}
// Making a new card should set the next card to the null pointer by default.
Test(make_card, next_is_null) {
  card* card_ = make_card('R', 1);
  cr_expect(eq(ptr, card_->next, NULL), "Wrong pointer!");
  free_card(card_);
}

// There aren't good, reliable ways to check free_card other than Valgrind, so
// don't add any unit tests for free_card.

//Making a new deck should create an empty deck, so the head should be the null
//pointer.
Test(make_deck, null_head) {
  deck* deck_ = make_deck();
  cr_assert(eq(ptr, deck_->head, NULL));
  free_deck(deck_);
}

// Making a new deck should create an empty deck, so the size should be 0.
Test(make_deck, zero_len) {
  deck* deck_ = make_deck();
  cr_assert(zero(sz, deck_->size));
  free_deck(deck_);
}

// Appending to a list should increase the length by one.
Test(append_card, increment_length) {
  deck* deck_ = make_deck();
  cr_assert(zero(sz, deck_->size));
  append_card(deck_, 'R', 1);
  cr_assert(eq(sz, deck_->size, 1));
  free_deck(deck_);
}

// Appending to a empty list should set the card to the head.
Test(append_card, append_empty) {
  deck* deck_ = make_deck();
  append_card(deck_, 'R', 1);
   card* head = deck_->head;
   cr_expect(eq(sz, deck_->head->value, 1), "Wrong value! Expected %zu, got %zu", 1,
          deck_->head->value);
   cr_expect(eq(chr, deck_->head->color, 'R'));
   cr_expect(eq(ptr, deck_->head->next, NULL));
  free_deck(deck_);
}

// Appending to a list should increase the length by two.
Test(append_card, append_two_elements) {
  deck* deck_ = make_deck();
  append_card(deck_, 'R', 1);
  append_card(deck_,'R', 2);
  cr_expect(eq(sz, deck_->size, 2));
  cr_expect(eq(chr, deck_->head->color, 'R'));
  cr_expect (deck_->head->next->color == 'R');
  cr_expect(eq(sz, deck_->head->next->value, 2));
  cr_expect(eq(ptr, deck_->head->next->next, NULL));
  free_deck(deck_);
}

// Appending to a deck should add the correct value at the correct index.
Test(append_card, correct_value_added) {
  deck* setup_deck = make_deck();
  append_card(setup_deck, 'R', 1);
  append_card(setup_deck,'W', 2);
  append_card(setup_deck,'G', 3);
  append_card(setup_deck,'B', 4);
  card* prev = get_card_index(setup_deck, 2); //Should we make test cases for get_card_index?
  card* final = get_card_index(setup_deck, 3);
  cr_expect(eq(ptr, prev->next, final));
  cr_expect(eq(sz, final->value, 4));
  cr_expect(eq(chr, final->color, 'B'));
  cr_expect(eq(ptr, final->next, NULL));
  free_deck(setup_deck);
}

//Test that if when card moves, deck size properly updates
Test(move_card, deck_size) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 1);
  append_card(deck_1, 'Y', 2);
  append_card(deck_1, 'G', 3);

  deck* deck_2 = make_deck();
  append_card(deck_2, 'R', 7);
  append_card(deck_2, 'R', 8);
  move_card(deck_1->head, deck_1, deck_2);
  cr_expect(eq(sz, deck_1->size, 2));
  cr_expect(eq(sz, deck_2->size, 3));
  free_deck(deck_1);
  free_deck(deck_2);
}


// Tests that if the card is the head that it properly moves.
Test(move_card, card_head) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 1);
  append_card(deck_1, 'Y', 2);
  append_card(deck_1, 'G', 3);
  deck* deck_2 = make_deck();
  append_card(deck_2, 'R', 7);
  move_card(deck_1->head, deck_1, deck_2);
  cr_expect(eq(chr, deck_1->head->color, 'Y'));
  cr_expect(eq(chr, deck_2->head->color, 'R'));
  cr_expect(eq(chr, deck_2->head->next->color, 'B'));
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
  cr_expect(zero(sz, deck_1->size));
  cr_expect(eq(ptr, deck_1->head, NULL));
  cr_expect(eq(sz, deck_2->size, 2));
  cr_expect(eq(chr, deck_2->head->next->color, 'B'));
  free_deck(deck_1);
  free_deck(deck_2);
}
// Tests that if the card is in the middle of the list it properly moves.
Test(move_card, card_middle) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 1);
  append_card(deck_1, 'Y', 2);
  append_card(deck_1, 'G', 3);

  deck* deck_2 = make_deck();
  append_card(deck_2, 'R', 4);
  move_card(deck_1->head->next, deck_1, deck_2);
  cr_assert(eq(sz, deck_1->size, 2));
  cr_assert(eq(sz, deck_2->size, 2));
  cr_expect(eq(chr, deck_1->head->color, 'B'));
  cr_expect(eq(chr, deck_1->head->next->color, 'G'));
  cr_expect(eq(chr, deck_2->head->next->color, 'Y'));
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
  cr_assert(eq(sz, deck_1->size, 2));
  cr_assert(eq(sz, deck_2->size, 1));
  cr_expect(eq(chr, deck_1->head->color, 'Y'));
  cr_expect(eq(chr, deck_2->head->color, 'B'));
  free_deck(deck_1);
  free_deck(deck_2);
}
// // Tests that if the first list is one and the second list is empty it moves
// // correctly.
Test(move_card, empty_second) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);
  deck* deck_2 = make_deck();
  move_card(deck_1->head, deck_1, deck_2);
  cr_assert(zero(sz, deck_1->size));
  cr_assert(eq(sz, deck_2->size, 1));
  cr_expect(eq(chr, deck_2->head->color, 'B'));
  free_deck(deck_1);
  free_deck(deck_2);
}


Test(make_UNO_deck, check_size ) {
  deck* uno = make_UNO_deck();
  cr_assert(eq(sz, uno->size, 108));
  free_deck(uno);
}

Test(make_UNO_deck, check_color ) {
  deck* uno = make_UNO_deck();
  cr_expect(eq(chr, uno->head->color, 'R'));
  free_deck(uno);
}

Test(make_UNO_deck, check_ratio) {
  deck* uno = make_UNO_deck();
  size_t red = 0;
  size_t blue = 0;
  size_t yellow = 0; 
  size_t green = 0;
  size_t wild = 0;
  card* current = uno->head; 
  while(current != NULL){
    if(current->color == 'R'){
      red++;
    }
    else if(current->color == 'B'){
      blue++;
    }
    else if(current->color == 'Y'){
      yellow++;
    }
    else if(current->color == 'G'){
      green++;
    }
    else if(current->color == 'W'){
      wild++;
    }
    current = current->next;
  }
  cr_assert(eq(sz, red, 25));
  cr_assert(eq(sz, blue, 25));
  cr_assert(eq(sz, yellow, 25));
  cr_assert(eq(sz, green, 25));
  cr_assert(eq(sz, wild, 8));
  free_deck(uno);
}

Test(shuffle, check_size) {
  deck* deck_1 = make_deck();
  append_card(deck_1, 'B', 7);
  append_card(deck_1, 'Y', 7);
  append_card(deck_1, 'G', 7);
  shuffle(deck_1);
  cr_assert(eq(sz, deck_1->size, 3));
}

// Test(shuffle, check_traverse) {
//   deck* deck_1 = make_deck();
//   append_card(deck_1, 'B', 7);
//   append_card(deck_1, 'Y', 7);
//   append_card(deck_1, 'G', 7);
//   shuffle(deck_1);
//   card* current = deck_1->head;
//   while (current->next != NULL) {
//     current = current->next;
//   }
//   cr_expect(eq(ptr, current->next, NULL));
// }

// Test(shuffle, check_shuffle) {
//   deck* deck_1 = make_deck();
//   append_card(deck_1, 'B', 7);
//   append_card(deck_1, 'Y', 7);
//   append_card(deck_1, 'G', 7);
//   shuffle(deck_1);
//   cr_assert(deck_1->head->color != 'B');
//   cr_assert(deck_1->head->next->color != 'Y');
//   cr_assert(deck_1->head->next->next->color != 'G');
// }


// Test(move_card, empty_second) {
//   deck* deck_1 = make_deck();
//   append_card(deck_1, 'B', 7);

//   deck* deck_2 = make_deck();
//   move_card(deck_1->head, deck_1, deck_2);
//   cr_assert(zero(sz, deck_1->size));
//   cr_assert(eq(sz, deck_2->size, 1));
//   cr_assert(deck_2->head->color == 'B');
//   free_deck(deck_1);
//   free_deck(deck_2);
// }
// Test(move_card, empty_second) {
//   deck* deck_1 = make_deck();
//   append_card(deck_1, 'B', 7);

//   deck* deck_2 = make_deck();
//   move_card(deck_1->head, deck_1, deck_2);
//   cr_assert(zero(sz, deck_1->size));
//   cr_assert(eq(sz, deck_2->size, 1));
//   cr_assert(deck_2->head->color == 'B');
//   free_deck(deck_1);
//   free_deck(deck_2);
// }
// Test(move_card, empty_second) {
//   deck* deck_1 = make_deck();
//   append_card(deck_1, 'B', 7);

//   deck* deck_2 = make_deck();
//   move_card(deck_1->head, deck_1, deck_2);
//   cr_assert(zero(sz, deck_1->size));
//   cr_assert(eq(sz, deck_2->size, 1));
//   cr_assert(deck_2->head->color == 'B');
//   free_deck(deck_1);
//   free_deck(deck_2);
// }
// Test(move_card, empty_second) {
//   deck* deck_1 = make_deck();
//   append_card(deck_1, 'B', 7);

//   deck* deck_2 = make_deck();
//   move_card(deck_1->head, deck_1, deck_2);
//   cr_assert(zero(sz, deck_1->size));
//   cr_assert(eq(sz, deck_2->size, 1));
//   cr_assert(deck_2->head->color == 'B');
//   free_deck(deck_1);
//   free_deck(deck_2);
// }


// // NOLINTEND(*-magic-numbers)

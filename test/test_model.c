#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>

#include "../src/model.h"

// NOLINTBEGIN(*-magic-numbers)

// When making a card, the color should be set correctly.
Test(make_card, color_set_correctly) {
  card* card_ = make_card('R', 1);
  cr_expect(eq(chr, card_->color, 'R'), "Wrong color! Expected %c, got %c", 'R',
            card_->color);
  free_card(card_);
}
// When making a card, the value should be set correctly.
Test(make_card, value_set_correctly) {
  card* card_ = make_card('R', 1);
  cr_expect(eq(sz, card_->value, 1), "Wrong value! Expected %zu, got %zu", 1,
            card_->value);
  free_card(card_);
}
// When making a card, the color and value should be set correctly.
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

// Appending to a deck should increase the length by one.
Test(append_card, increment_length) {
  deck deck_ = {.head = NULL, .size = 0};
  cr_assert(zero(sz, deck_.size));
  append_card(&deck_, 'R', 1);
  cr_assert(eq(sz, deck_.size, 1));
  free_deck(&deck_);
}

// Appending to a empty deck should set the card to the head.
Test(append_card, append_empty) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'R', 1);
  cr_expect(eq(sz, deck_.head->value, 1), "Wrong value! Expected %zu, got %zu",
            1, deck_.head->value);
  cr_expect(eq(chr, deck_.head->color, 'R'));
  cr_expect(eq(ptr, deck_.head->next, NULL));
  free_deck(&deck_);
}

// Appending to a deck twice should increase the length by two.
Test(append_card, append_two_elements) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'R', 1);
  append_card(&deck_, 'R', 2);
  cr_expect(eq(sz, deck_.size, 2));
  cr_expect(eq(chr, deck_.head->color, 'R'));
  cr_expect(deck_.head->next->color == 'R');
  cr_expect(eq(sz, deck_.head->next->value, 2));
  cr_expect(eq(ptr, deck_.head->next->next, NULL));
  free_deck(&deck_);
}

// Appending to a deck should add the correct value at the correct index.
Test(append_card, correct_value_added) {
  deck setup_deck = {.head = NULL, .size = 0};
  append_card(&setup_deck, 'R', 1);
  append_card(&setup_deck, 'W', 2);
  append_card(&setup_deck, 'G', 3);
  append_card(&setup_deck, 'B', 4);
  card* prev = get_card_index(
      &setup_deck, 2);  // Should we make test cases for get_card_index?
  card* final = get_card_index(&setup_deck, 3);
  cr_expect(eq(ptr, prev->next, final));
  cr_expect(eq(sz, final->value, 4));
  cr_expect(eq(chr, final->color, 'B'));
  cr_expect(eq(ptr, final->next, NULL));
  free_deck(&setup_deck);
}

// Test that if when card moves, deck size properly updates
Test(move_card, deck_size) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 1);
  append_card(&deck_1, 'Y', 2);
  append_card(&deck_1, 'G', 3);

  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'R', 7);
  append_card(&deck_2, 'R', 8);
  move_card(deck_1.head, &deck_1, &deck_2);
  cr_expect(eq(sz, deck_1.size, 2));
  cr_expect(eq(sz, deck_2.size, 3));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

// Tests that if the card is the head that it properly moves.
Test(move_card, card_head) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 1);
  append_card(&deck_1, 'Y', 2);
  append_card(&deck_1, 'G', 3);
  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'R', 7);
  move_card(deck_1.head, &deck_1, &deck_2);
  cr_expect(eq(chr, deck_1.head->color, 'Y'));
  cr_expect(eq(chr, deck_2.head->color, 'R'));
  cr_expect(eq(chr, deck_2.head->next->color, 'B'));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

// Tests that if the card is the head and its next is null it properly moves.
Test(move_card, card_null) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);

  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'R', 7);
  move_card(deck_1.head, &deck_1, &deck_2);
  cr_expect(zero(sz, deck_1.size));
  cr_expect(eq(ptr, deck_1.head, NULL));
  cr_expect(eq(sz, deck_2.size, 2));
  cr_expect(eq(chr, deck_2.head->next->color, 'B'));
  free_deck(&deck_1);
  free_deck(&deck_2);
}
// Tests that if the card is in the middle of the deck it properly moves.
Test(move_card, card_middle) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 1);
  append_card(&deck_1, 'Y', 2);
  append_card(&deck_1, 'G', 3);

  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'R', 4);
  move_card(deck_1.head->next, &deck_1, &deck_2);
  cr_assert(eq(sz, deck_1.size, 2));
  cr_assert(eq(sz, deck_2.size, 2));
  cr_expect(eq(chr, deck_1.head->color, 'B'));
  cr_expect(eq(chr, deck_1.head->next->color, 'G'));
  cr_expect(eq(chr, deck_2.head->next->color, 'Y'));
  free_deck(&deck_1);
  free_deck(&deck_2);
}
// Tests that if the second deck is empty it properly moves.
Test(move_card, empty_deck) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);
  append_card(&deck_1, 'Y', 7);
  append_card(&deck_1, 'G', 7);

  deck deck_2 = {.head = NULL, .size = 0};
  move_card(deck_1.head, &deck_1, &deck_2);
  cr_assert(eq(sz, deck_1.size, 2));
  cr_assert(eq(sz, deck_2.size, 1));
  cr_expect(eq(chr, deck_1.head->color, 'Y'));
  cr_expect(eq(chr, deck_2.head->color, 'B'));
  free_deck(&deck_1);
  free_deck(&deck_2);
}
// // Tests that if the first deck is one and the second deck is empty it moves
// // correctly.
Test(move_card, empty_second) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);
  deck deck_2 = {.head = NULL, .size = 0};
  move_card(deck_1.head, &deck_1, &deck_2);
  cr_assert(zero(sz, deck_1.size));
  cr_assert(eq(sz, deck_2.size, 1));
  cr_expect(eq(chr, deck_2.head->color, 'B'));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

Test(make_uno_deck, check_size) {
  deck uno = make_uno_deck();
  cr_assert(eq(sz, uno.size, 108));
  free_deck(&uno);
}

Test(make_uno_deck, check_color) {
  deck uno = make_uno_deck();
  cr_expect(eq(chr, uno.head->color, 'R'));
  free_deck(&uno);
}

Test(make_uno_deck, check_ratio) {
  deck uno = make_uno_deck();
  size_t red = 0;
  size_t blue = 0;
  size_t yellow = 0;
  size_t green = 0;
  size_t wild = 0;
  card* current = uno.head;
  while (current != NULL) {
    if (current->color == 'R') {
      red++;
    } else if (current->color == 'B') {
      blue++;
    } else if (current->color == 'Y') {
      yellow++;
    } else if (current->color == 'G') {
      green++;
    } else if (current->color == 'W') {
      wild++;
    }
    current = current->next;
  }
  cr_assert(eq(sz, red, 25));
  cr_assert(eq(sz, blue, 25));
  cr_assert(eq(sz, yellow, 25));
  cr_assert(eq(sz, green, 25));
  cr_assert(eq(sz, wild, 8));
  free_deck(&uno);
}

// Empty decks should be equal.
Test(equal, empty_decks_equal) {
  deck lhs = {.head = NULL, .size = 0};
  deck rhs = {.head = NULL, .size = 0};
  cr_assert(eq(int, equal(&lhs, &rhs), 1));
  free_deck(&lhs);
  free_deck(&rhs);
}

// Any nonempty deck should be equal to itself.
Test(equal, reflexive_deck) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'R', 1);
  cr_assert(eq(int, equal(&deck_, &deck_), 1));
  free_deck(&deck_);
}

// Two decks with different lengths should not be equal.
Test(equal, different_lengths) {
  deck lhs = {.head = NULL, .size = 0};
  deck rhs = {.head = NULL, .size = 0};
  append_card(&rhs, 'R', 1);
  cr_assert(zero(int, equal(&lhs, &rhs)));
  free_deck(&lhs);
  free_deck(&rhs);
}

// Two decks with the same lengths and different elements should not be equal.
Test(equal, different_elements) {
  deck lhs = {.head = NULL, .size = 0};
  append_card(&lhs, 'R', 0);
  deck rhs = {.head = NULL, .size = 0};
  append_card(&rhs, 'R', 1);
  cr_assert(zero(int, equal(&lhs, &rhs)));
  free_deck(&lhs);
  free_deck(&rhs);
}

// Two decks with the same elements in different orders should not be equal.
Test(equal, different_element_order) {
  deck lhs = {.head = NULL, .size = 0};
  append_card(&lhs, 'R', 1);
  append_card(&lhs, 'R', 2);
  append_card(&lhs, 'R', 3);
  deck rhs = {.head = NULL, .size = 0};
  append_card(&rhs, 'R', 1);
  append_card(&rhs, 'R', 3);
  append_card(&rhs, 'R', 2);
  cr_assert(zero(int, equal(&lhs, &rhs)));
  free_deck(&lhs);
  free_deck(&rhs);
}

// Two decks with the same elements in the same order should be equal.
Test(equal, deck_copy) {
  deck lhs = {.head = NULL, .size = 0};
  append_card(&lhs, 'R', 1);
  append_card(&lhs, 'R', 2);
  append_card(&lhs, 'R', 3);
  deck rhs = {.head = NULL, .size = 0};
  append_card(&rhs, 'R', 1);
  append_card(&rhs, 'R', 2);
  append_card(&rhs, 'R', 3);
  cr_assert(eq(int, equal(&lhs, &rhs), 1));
  free_deck(&lhs);
  free_deck(&rhs);
}

// Two decks with the same values but different colors should not be  in the
// same order should be equal.
Test(equal, deck_color) {
  deck lhs = {.head = NULL, .size = 0};
  append_card(&lhs, 'R', 1);
  append_card(&lhs, 'R', 2);
  append_card(&lhs, 'R', 3);
  deck rhs = {.head = NULL, .size = 0};
  append_card(&rhs, 'R', 1);
  append_card(&rhs, 'G', 2);
  append_card(&rhs, 'R', 3);
  cr_assert(eq(int, equal(&lhs, &rhs), 0));
  free_deck(&lhs);
  free_deck(&rhs);
}

// A deck should be the same size after being shuffled.
Test(shuffle, check_size) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);
  append_card(&deck_1, 'Y', 7);
  append_card(&deck_1, 'G', 7);
  shuffle(&deck_1);
  cr_assert(eq(sz, deck_1.size, 3));
  free_deck(&deck_1);
}

// An empty list shuffled should be the same.
Test(shuffle, check_empty) {
  deck deck_1 = {.head = NULL, .size = 0};
  deck deck_2 = {.head = NULL, .size = 0};
  shuffle(&deck_1);
  cr_assert(eq(int, equal(&deck_1, &deck_2), 1));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

// A list of 1 card shuffled should be the same.
Test(shuffle, check_one_card) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);
  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'B', 7);
  shuffle(&deck_1);
  cr_assert(eq(int, equal(&deck_1, &deck_2), 1));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

// UNO shuffled and UNO created should not be equal.
Test(shuffle, check_uno) {
  deck uno_1 = make_uno_deck();
  card* current = uno_1.head;
  deck uno_2 = make_uno_deck();
  shuffle(&uno_1);
  cr_assert(eq(sz, uno_1.size, 108));
  cr_assert(eq(int, equal(&uno_1, &uno_2), 0));
  free_deck(&uno_1);
  free_deck(&uno_2);
}
// A small list shuffled should not be the same as the deck before.
Test(shuffle, check_deck) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);
  append_card(&deck_1, 'Y', 7);
  append_card(&deck_1, 'G', 7);
  append_card(&deck_1, 'B', 1);
  append_card(&deck_1, 'Y', 2);
  append_card(&deck_1, 'G', 3);
  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'B', 7);
  append_card(&deck_2, 'Y', 7);
  append_card(&deck_2, 'G', 7);
  append_card(&deck_2, 'B', 1);
  append_card(&deck_2, 'Y', 2);
  append_card(&deck_2, 'G', 3);
  shuffle(&deck_1);
  cr_assert(eq(int, equal(&deck_1, &deck_2), 0));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

// Test that if when card moves, deck size properly updates
Test(get_card_index, index_zero) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* get = get_card_index(&deck_, 0);
  cr_expect(eq(ptr, get, deck_.head), "Wrong pointer!");
  free_deck(&deck_);
}

// Test that if when card moves, deck size properly updates
Test(get_card_index, index_one) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* get = get_card_index(&deck_, 1);
  cr_expect(eq(ptr, get, deck_.head->next), "Wrong pointer!");
  free_deck(&deck_);
}

Test(get_card_index, index_two) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* get = get_card_index(&deck_, 2);
  cr_expect(eq(ptr, get, deck_.head->next->next), "Wrong pointer!");
  free_deck(&deck_);
}

Test(get_card_index, index_too_big) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* get = get_card_index(&deck_, 5);
  cr_expect(eq(ptr, get, NULL), "Wrong pointer!");
  free_deck(&deck_);
}

Test(find_card, head) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* find = find_card(&deck_, 'B', 1);
  cr_expect(eq(ptr, find, deck_.head), "Wrong pointer!");
  free_deck(&deck_);
}

Test(find_card, middle) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* find = find_card(&deck_, 'Y', 2);
  cr_expect(eq(ptr, find, deck_.head->next), "Wrong pointer!");
  free_deck(&deck_);
}

Test(find_card, end) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* find = find_card(&deck_, 'G', 3);
  cr_expect(eq(ptr, find, deck_.head->next->next), "Wrong pointer!");
  free_deck(&deck_);
}

Test(find_card, not_found) {
  deck deck_ = {.head = NULL, .size = 0};
  append_card(&deck_, 'B', 1);
  append_card(&deck_, 'Y', 2);
  append_card(&deck_, 'G', 3);
  card* find = find_card(&deck_, 'B', 3);
  cr_expect(eq(ptr, find, NULL), "Wrong pointer!");
  free_deck(&deck_);
}

// A small list shuffled should not be the same as the deck before.
Test(append_deck, two_decks) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'B', 7);
  append_card(&deck_1, 'Y', 7);
  append_card(&deck_1, 'G', 7);
  append_card(&deck_1, 'B', 1);
  append_card(&deck_1, 'Y', 2);
  append_card(&deck_1, 'G', 3);
  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'B', 7);
  append_card(&deck_2, 'Y', 7);
  append_card(&deck_2, 'G', 7);
  append_card(&deck_2, 'B', 1);
  append_card(&deck_2, 'Y', 2);
  append_card(&deck_2, 'G', 3);
  append_deck(&deck_1, &deck_2);
  cr_assert(eq(sz, deck_1.size, 0));
  cr_assert(eq(sz, deck_2.size, 12));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

Test(append_deck, empty_deck) {
  deck deck_1 = {.head = NULL, .size = 0};
  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'B', 7);
  append_card(&deck_2, 'Y', 7);
  append_deck(&deck_1, &deck_2);
  cr_assert(eq(sz, deck_1.size, 0));
  cr_assert(eq(sz, deck_2.size, 2));
  free_deck(&deck_1);
  free_deck(&deck_2);
}
Test(append_deck, pointers_set_correctly) {
  deck deck_1 = {.head = NULL, .size = 0};
  append_card(&deck_1, 'G', 7);
  deck deck_2 = {.head = NULL, .size = 0};
  append_card(&deck_2, 'B', 7);
  append_card(&deck_2, 'Y', 7);
  card* old_head = deck_1.head;
  append_deck(&deck_1, &deck_2);
  cr_assert(eq(sz, deck_1.size, 0));
  cr_assert(eq(sz, deck_2.size, 3));
  cr_assert(eq(ptr, old_head, deck_2.head->next->next));
  free_deck(&deck_1);
  free_deck(&deck_2);
}

Test(check_draw, needs_refill_1) {
  game_state* state = make_game_state();
  append_deck(&state->draw, &state->discard);
  cr_assert(eq(int, check_draw(state), 1));
  free_game_state(state);
}
Test(check_draw, needs_refill_2) {
  game_state* state = make_game_state();
  append_deck(&state->draw, &state->discard);
  move_card(state->discard.head,&state->discard,&state->draw );
  cr_assert(eq(sz, state->draw.size, 1));
  cr_assert(eq(int, check_draw(state), 1));
  free_game_state(state);
}
Test(check_draw, no_refill) {
  game_state* state = make_game_state();
  cr_assert(eq(int, check_draw(state), 0));
  free_game_state(state);
}

Test(refill_draw, empty_draw) {
  game_state* state = make_game_state();
  deck uno = make_uno_deck();
  append_deck(&state->draw, &state->discard);
  cr_assert(eq(sz, state->draw.size, 0));
  cr_assert(eq(sz, state->discard.size, 107));
  refill_draw(state);
  cr_assert(eq(sz, state->discard.size, 0));
  cr_assert(eq(sz, state->draw.size, 107));
  cr_assert(eq(int, equal(&uno, &(state->draw)), 0));
  free_deck(&uno);
  free_game_state(state);
}
Test(refill_draw, one_draw) {
  game_state* state = make_game_state();
  deck uno = make_uno_deck();
  append_deck(&state->draw, &state->discard);
  move_card(state->discard.head, &state->discard, &state->draw);
  cr_assert(eq(sz, state->draw.size, 1));
  refill_draw(state);
  cr_assert(eq(sz, state->discard.size, 0));
  cr_assert(eq(sz, state->draw.size, 107));
  free_deck(&uno);
  free_game_state(state);
}

// When making a card, the color and value should be set correctly.
Test(make_player, values_set_correctly) {
  player* player_ = make_player(0);
  cr_expect(eq(ptr, player_->next, NULL), "Wrong pointer!");
  cr_expect(eq(ptr, player_->prev, NULL), "Wrong pointer!");
  cr_expect(eq(int, player_->sock_num, -1));
  cr_expect(eq(sz, player_->number, 0));
  free_player(player_);
}

// Making a new deck should create an empty deck, so the head should be the null
// pointer.
Test(make_order, one_player) {
  order* order = make_order(1);
  cr_assert(eq(ptr, order->head->next, order->head));
  cr_assert(eq(sz, order->direction, 0));
  free_order(order);
}

Test(make_order, two_player_numbers) {
  order* order = make_order(2);
  player* one = order->head;
  player* two = one->next;
  cr_assert(eq(sz, one->number, 0));
  cr_assert(eq(sz, two->number, 1));
  free_order(order);
}

Test(make_order, two_player_pointers) {
  order* order = make_order(2);
  player* one = order->head;
  player* two = one->next;
  cr_assert(eq(ptr, one->next, two));
  cr_assert(eq(ptr, one->prev, two));
  cr_assert(eq(ptr, two->next, one));
  cr_assert(eq(ptr, two->prev, one));
  free_order(order);
}

Test(make_order, three_player_numbers) {
  order* order = make_order(3);
  player* one = order->head;
  player* two = one->next;
  player* three = two->next;
  cr_assert(eq(sz, one->number, 0));
  cr_assert(eq(sz, two->number, 1));
  cr_assert(eq(sz, three->number, 2));
  free_order(order);
}

Test(make_order, three_player_pointers) {
  order* order = make_order(3);
  player* one = order->head;
  player* two = one->next;
  player* three = two->next;
  cr_assert(eq(ptr, one->next, two));
  cr_assert(eq(ptr, one->prev, three));
  cr_assert(eq(ptr, two->next, three));
  cr_assert(eq(ptr, two->prev, one));
  cr_assert(eq(ptr, three->next, one));
  cr_assert(eq(ptr, three->prev, two));
  free_order(order);
}

Test(make_order, four_player_numbers) {
  order* order = make_order(4);
  player* one = order->head;
  player* two = one->next;
  player* three = two->next;
  player* four = three->next;
  cr_assert(eq(sz, one->number, 0));
  cr_assert(eq(sz, two->number, 1));
  cr_assert(eq(sz, three->number, 2));
  cr_assert(eq(sz, four->number, 3));
  free_order(order);
}

Test(make_order, four_player_pointers) {
  order* order = make_order(4);
  player* one = order->head;
  player* two = one->next;
  player* three = two->next;
  player* four = three->next;
  cr_assert(eq(ptr, one->next, two));
  cr_assert(eq(ptr, one->prev, four));
  cr_assert(eq(ptr, two->next, three));
  cr_assert(eq(ptr, two->prev, one));
  cr_assert(eq(ptr, three->next, four));
  cr_assert(eq(ptr, three->prev, two));
  cr_assert(eq(ptr, four->next, one));
  cr_assert(eq(ptr, four->prev, three));
  free_order(order);
}

Test(make_order, five_player_numbers) {
  order* order = make_order(5);
  player* one = order->head;
  player* two = one->next;
  player* three = two->next;
  player* four = three->next;
  player* five = four->next;
  cr_assert(eq(sz, one->number, 0));
  cr_assert(eq(sz, two->number, 1));
  cr_assert(eq(sz, three->number, 2));
  cr_assert(eq(sz, four->number, 3));
  cr_assert(eq(sz, five->number, 4));
  free_order(order);
}

Test(make_order, five_player_pointers) {
  order* order = make_order(5);
  player* one = order->head;
  player* two = one->next;
  player* three = two->next;
  player* four = three->next;
  player* five = four->next;
  cr_assert(eq(ptr, one->next, two));
  cr_assert(eq(ptr, one->prev, five));
  cr_assert(eq(ptr, two->next, three));
  cr_assert(eq(ptr, two->prev, one));
  cr_assert(eq(ptr, three->next, four));
  cr_assert(eq(ptr, three->prev, two));
  cr_assert(eq(ptr, four->next, five));
  cr_assert(eq(ptr, four->prev, three));
  cr_assert(eq(ptr, five->next, one));
  cr_assert(eq(ptr, five->prev, four));
  free_order(order);
}

// When making a card, the color and value should be set correctly.
Test(make_hand, size) {
  game_state* state = make_game_state();
  state->player_list = make_order(1);
  make_hand(state, state->player_list->head);
  cr_expect(eq(sz, state->player_list->head->hand.size, 7));
  cr_expect(eq(sz, state->draw.size, 100));
  free_game_state(state);
}

Test(switch_direction, check_direction) {
  game_state* state = make_game_state();
  state->player_list = make_order(1);
  switch_direction(state);
  cr_expect(eq(sz, state->player_list->direction, 1));
  switch_direction(state);
  cr_expect(eq(sz, state->player_list->direction, 0));
  free_game_state(state);
}

Test(skip, check_turn) {
  game_state* state = make_game_state();
  state->player_list = make_order(3);
  state->turn = state->player_list->head;
  skip(state);
  cr_expect(eq(sz, state->turn->number, 2));
  free_game_state(state);
}

Test(in_hand, fir_in_hand) {
  // check that a card that's the first card in the hand returns 1
  card* played_card = make_card('B', 4);
  deck hand = {.head = NULL, .size = 0};
  hand.head = played_card;
  cr_assert(eq(int, in_hand(played_card, &hand), 1));
  free_deck(&hand);
}

Test(in_hand, sec_in_hand) {
  // check that a card that's not the first card in the hand returns 1
  card* played_card = make_card('B', 4);
  deck hand = {.head = NULL, .size = 0};
  hand.head = make_card('Y', 5);
  hand.head->next = played_card;

  cr_assert(eq(int, in_hand(played_card, &hand), 1));
  free_deck(&hand);
}

Test(in_hand, right_col_wrong_val) {
  // check that a card that's the same color as one of the cards in the hand,
  // but the wrong value returns 0
  card* played_card = make_card('B', 4);
  deck hand = {.head = NULL, .size = 0};
  hand.head = make_card('B', 8);
  hand.head->next = make_card('Y', 3);
  cr_assert(zero(int, in_hand(played_card, &hand)));
  free_deck(&hand);
}

Test(in_hand, wrong_col_wrong_val) {
  // check that a card that's the same value as one of the cards in the hand but
  // the wrong color returns 0
  card* played_card = make_card('B', 4);
  deck hand = {.head = NULL, .size = 0};
  hand.head = make_card('R', 4);
  hand.head->next = make_card('Y', 3);
  cr_assert(zero(int, in_hand(played_card, &hand)));
  free_deck(&hand);
}

Test(in_hand, all_wrong) {
  // check that a card that's not in the hand returns 0
  card* played_card = make_card('B', 4);
  deck hand = {.head = NULL, .size = 0};
  hand.head = make_card('R', 2);
  hand.head->next = make_card('Y', 3);
  cr_assert(zero(int, in_hand(played_card, &hand)));
  free_deck(&hand);
}

Test(is_valid, not_in_hand) {
  // check that a card that is not in the hand returns 0
  char card_str[5];
  strcpy(card_str, "B4");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('G', 4);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(zero(int, is_valid(card_str, &game_state_)));
  free_deck(&hand_);
}

Test(is_valid, in_hand_right_color) {
  // check that a non special card that's the right color and in the hand
  // returns 1
  char card_str[5];
  strcpy(card_str, "B4");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 4);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_col_rev) {
  // check that a reverse card that's the right color and in the hand returns 1
  char card_str[5];
  strcpy(card_str, "B10");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 10);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_col_skip) {
  // check that a skip card that's the right color and in the hand returns 1
  char card_str[5];
  strcpy(card_str, "B11");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 11);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_col_draw) {
  // check that a draw 2 card that's the right color and in the hand returns 1
  char card_str[5];
  strcpy(card_str, "B12");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 12);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_val_wrong_col) {
  // check that a nonspecial card with the same value as the top card but a diff
  // color returns 1
  char card_str[5];
  strcpy(card_str, "B3");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 3);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('R', 3);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_val_rev) {
  // check that a rev card on top of a rev card returns 1
  char card_str[5];
  strcpy(card_str, "B10");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 10);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('R', 10);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_val_skip) {
  // check that a skip card on top of a skip card returns 1
  char card_str[5];
  strcpy(card_str, "B11");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 11);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, right_val_draw) {
  // check that a draw 2 card on top of another draw 2 card returns 1
  char card_str[5];
  strcpy(card_str, "B12");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 12);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('R', 12);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, draw_four) {
  // check that a draw four card returns 1
  char card_str[5];
  strcpy(card_str, "B13");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 13);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, wildcard) {
  // check that a wildcard returns 1
  char card_str[5];
  strcpy(card_str, "B14");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 14);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('B', 8);
  cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
  free_deck(&hand_);
}

Test(is_valid, in_hand_no_match) {
  // check that a card that's in a player's hand but isn't the same color or
  // value as the top card returns 0
  char card_str[5];
  strcpy(card_str, "B8");
  deck hand_ = {.head = NULL, .size = 0};
  hand_.head = make_card('Y', 5);
  hand_.head->next = make_card('B', 8);

  game_state game_state_;
  game_state_.turn = make_player(1);
  game_state_.turn->hand = hand_;
  game_state_.main.head = make_card('Y', 7);
  cr_assert(zero(int, is_valid(card_str, &game_state_)));
  free_deck(&hand_);
}

Test(change_turn, moving_next) {
  // check that if the direction is zero, then the new player after changing
  // directions is the next player
  game_state* game_state_ = make_game_state();
  game_state_->player_list = make_order(3);
  game_state_->turn = game_state_->player_list->head;
  change_turn(game_state_);
  cr_assert(eq(sz, game_state_->turn->number, 1));
  free_game_state(game_state_);
}

Test(change_turn, moving_prev) {
  // check that if the direction is zero, then the new player after changing
  // directions is the previous player
  game_state* game_state_ = make_game_state();
  game_state_->player_list = make_order(3);
  game_state_->player_list->direction = 1;
  game_state_->turn = game_state_->player_list->head;
  change_turn(game_state_);
  // printf("%i\n", game_state_.turn.number);
  cr_assert(eq(sz, game_state_->turn->number, 2));
  free_game_state(game_state_);
}

Test(draw, checks_player_hand) {
  game_state* game_state_ = make_game_state();
  game_state_->player_list = make_order(1);
  draw(game_state_, game_state_->player_list->head);
  draw(game_state_, game_state_->player_list->head);
  draw(game_state_, game_state_->player_list->head);
  cr_expect(eq(sz, game_state_->player_list->head->hand.size, 3));
  free_game_state(game_state_);
}
Test(draw2, check_player_hand) {
  game_state* game_state_ = make_game_state();
  game_state_->player_list = make_order(3);
  draw2(game_state_, game_state_->player_list->head);
  cr_assert(eq(sz, game_state_->player_list->head->hand.size, 2));
  free_game_state(game_state_);
}

Test(draw4, check_player_hand) {
  game_state* game_state_ = make_game_state();
  game_state_->player_list = make_order(3);
  draw4(game_state_, game_state_->player_list->head);
  cr_assert(eq(sz, game_state_->player_list->head->hand.size, 4));
  free_game_state(game_state_);
}
// NOLINTEND(*-magic-numbers)

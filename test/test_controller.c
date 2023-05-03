#include "../src/controller.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#include <stdlib.h>

// NOLINTBEGIN(*-magic-numbers)

Test(process_input, reg_input) {
    // check that a regular non-special card input returns the same string
    char buf[3];
    strcpy(buf, "B9");
    char processed_input[3];
    process_input(buf, processed_input);
    cr_assert(eq(str, processed_input, buf)) ;
}

Test(process_input, rev_input) {
    // check that a reverse card input returns the color + '10' to indicate that it's a reverse card
    char buf[3];
    strcpy(buf, "Rr");
    char processed_input[4];
    process_input(buf, processed_input);
    cr_assert(eq(str, processed_input, "R10"));
}

Test(process_input, skip_input) {
    // check that a skip card input returns the color + '11' to indicate that it's a skip card
    char buf[3];
    strcpy(buf, "YS");
    char processed_input[4];
    process_input(buf, processed_input);
    cr_assert(eq(str, processed_input, "Y11"));
}

Test(process_input, draw_2_input) {
    // check that a draw 2 card returns the color + '12' to indicate that it's a draw 2 card
    char buf[3];
    strcpy(buf, "G2");
    char processed_input[4];
    process_input(buf, processed_input);
    cr_assert(eq(str, processed_input, "G12"));
}

Test(process_input, wild_input) {
    // check that a wild card input returns the color chose + '13' to indicate that it's a wild card
    char buf[3];
    strcpy(buf, "WR");
    char processed_input[4];
    process_input(buf, processed_input);
    cr_assert(eq(str, processed_input, "R13"));
}

Test(process_input, wild_4_input) {
    // check that a draw 4 wild card input returns the color chosen + '14' to indicate that it's a draw 4 wild card
    char buf[4];
    strcpy(buf, "W4Y");
    char processed_input[4];
    process_input(buf, processed_input);
    cr_assert(eq(str, processed_input, "Y14"));
}

Test(in_hand, fir_in_hand) {
    // check that a card that's the first card in the hand returns 1
    card* played_card = make_card('B', 4);
    deck* hand = make_deck();
    hand->head = played_card;
    cr_assert(eq(int, in_hand(played_card, hand), 1));
    free_deck(hand);
}

Test(in_hand, sec_in_hand) {
    // check that a card that's not the first card in the hand returns 1
    card* played_card = make_card('B', 4);
    deck* hand = make_deck();
    hand->head = make_card('Y', 5);
    hand->head->next = played_card;

    cr_assert(eq(int, in_hand(played_card, hand), 1));
    free_deck(hand);
}

Test(in_hand, right_col_wrong_val) {
    // check that a card that's the same color as one of the cards in the hand, but the wrong value returns 0
    card* played_card = make_card('B', 4);
    deck* hand = make_deck();
    hand->head = make_card('B', 8);
    hand->head->next = make_card('Y', 3);
    cr_assert(zero(int, in_hand(played_card, hand)));
    free_deck(hand);
}

Test(in_hand, wrong_col_wrong_val) {
    // check that a card that's the same value as one of the cards in the hand but the wrong color returns 0
    card* played_card = make_card('B', 4);
    deck* hand = make_deck();
    hand->head = make_card('R', 4);
    hand->head->next = make_card('Y', 3);
    cr_assert(zero(int, in_hand(played_card, hand)));
    free_deck(hand);
}

Test(in_hand, all_wrong) {
    // check that a card that's not in the hand returns 0
    card* played_card = make_card('B', 4);
    deck* hand = make_deck();
    hand->head = make_card('R', 2);
    hand->head->next = make_card('Y', 3);
    cr_assert(zero(int, in_hand(played_card, hand)));
    free_deck(hand);
}

Test(is_valid, not_in_hand) {
    // check that a card that is not in the hand returns 0
    char card_str[5];
    strcpy(card_str, "B4");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('G', 4);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(zero(int, is_valid(card_str, &game_state_)));
    free_deck(hand_);
}

Test(is_valid, in_hand_right_color) {
    // check that a non special card that's the right color and in the hand returns 1
    char card_str[5];
    strcpy(card_str, "B4");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 4);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_col_rev) {
    // check that a reverse card that's the right color and in the hand returns 1
    char card_str[5];
    strcpy(card_str, "B10");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 10);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_col_skip) {
    // check that a skip card that's the right color and in the hand returns 1
    char card_str[5];
    strcpy(card_str, "B11");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 11);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_col_draw) {
    // check that a draw 2 card that's the right color and in the hand returns 1
    char card_str[5];
    strcpy(card_str, "B12");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 12);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_val_wrong_col) {
    // check that a nonspecial card with the same value as the top card but a diff color returns 1
    char card_str[5];
    strcpy(card_str, "B3");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 3);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('R', 3);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_val_rev) {
    // check that a rev card on top of a rev card returns 1
    char card_str[5];
    strcpy(card_str, "B10");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 10);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('R', 10);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_val_skip) {
    // check that a skip card on top of a skip card returns 1
    char card_str[5];
    strcpy(card_str, "B11");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 11);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, right_val_draw) {
    // check that a draw 2 card on top of another draw 2 card returns 1
    char card_str[5];
    strcpy(card_str, "B12");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 12);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('R', 12);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, draw_four) {
    // check that a draw four card returns 1
    char card_str[5];
    strcpy(card_str, "B13");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 13);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, wildcard) {
    // check that a wildcard returns 1
    char card_str[5];
    strcpy(card_str, "B14");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 14);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('B', 8);
    cr_assert(eq(int, is_valid(card_str, &game_state_), 1));
    free_deck(hand_);
}

Test(is_valid, in_hand_no_match) {
    // check that a card that's in a player's hand but isn't the same color or value as the top card returns 0
    char card_str[5];
    strcpy(card_str, "B8");
    deck* hand_ = make_deck();
    hand_->head = make_card('Y', 5);
    hand_->head->next = make_card('B', 8);
    
    game_state game_state_;
    game_state_.turn = *make_player(1);
    game_state_.turn.hand = *hand_;
    game_state_.main.head = make_card('Y', 7);
    cr_assert(zero(int, is_valid(card_str, &game_state_)));
    free_deck(hand_);
}

// Test(change_turn, moving_next) {
//     // check that if the direction is zero, then the new player after changing directions is the next player
//     game_state game_state_;
//     game_state_.player_list = make_order(3);
//     game_state_.player_list->direction = 0;
//     change_turn(&game_state_);
//     cr_assert(eq(int, (int) game_state_.player_list->head->number, 1));
    
// }
// NOLINTEND(*-magic-numbers)

/*
 * If a player calls uno, make sure that it's valid. If it's their uno, do nothing. 
 * If it's someone else's uno, make that player draw 4 cards from the deck.
 * If no one has uno, the person who called uno has to draw 4 cards from the deck
 * 
 * @param player_: the player that called uno
 */
void call_uno(player* player_);

/*
 * Draw a card from the game deck
 *
 * @param player_: the player that is drawing a card
 * @param deck_: the deck from which the player is drawing a card from
 * 
 * @return card*: The card that has been drawn
 */
card* draw_card(player* player_, deck* deck_);

/*
 * Choose a card from a hand
 *
 * @param player_: the player that is picking a card (Would the player struct contain the hand?? I think so)
 * 
 * @return card*: the card that is being played
 */
card* choose_card(player* player_);

/*
 * Check if card played is a valid move. A move is valid if it's the same color or number as the top
 * card on the deck. Additionally, if it's a pick color card, then it's valid no matter what the top
 * card.
 * 
 * @param card_: the card that is being played
 * @param played_deck: the deck of cards that have been played. Only looking at the top card to check
 *                     if the move is valid
 * 
 * @return bool: returns true if the move is valid
 */
bool is_valid(card* card_, deck* played_deck);

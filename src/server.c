#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L  // NOLINT
#endif  // NOLINT(*bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
#define _GNU_SOURCE  // NOLINT(*bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include "server.h"

#include "utils.h"

uno_server* make_uno_server(struct sockaddr_in ip_addr, int max_backlog) {
  uno_server* server = malloc(sizeof(uno_server));
  server->listener = open_tcp_socket();
  server->addr = ip_addr;
  server->max_backlog = max_backlog;
  return server;
}

void free_uno_server(uno_server* server) {
  close_tcp_socket(server->listener);
  free(server);
}

void listen_for_connections(uno_server* server) {
  if (bind(server->listener, &server->addr, sizeof(server->addr)) == -1) {
    error_and_exit("Binding error");
  }
  if (listen(server->listener, server->max_backlog) == -1) {
    error_and_exit("Listening error");
  };
}

int accept_client(uno_server* server, game_state* game_state) {
  struct sockaddr client_addr;
  unsigned int address_size = sizeof(server->addr);
  int connected_d = accept(server->listener, &client_addr, &address_size);

  if (connected_d < 0) {
    error_and_exit("Connection to server failed");
  }

  if (game_state->current_players == 0) {
    // If the player who joined is the first player, it initializes the game
    // state
    game_state->number_players = 2;
    game_state->player_list = make_order(game_state->number_players);
    game_state->player_list->head->sock_num = connected_d;
    game_state->current_players++;
  } else {
    player* current = game_state->player_list->head;

    for (size_t i = 0; i < game_state->number_players; i++) {
      // Adds the player to the player list
      if (current->sock_num != -1) {
        current = current->next;
      } else {
        current->sock_num = connected_d;
        game_state->current_players = game_state->current_players + 1;
        break;
      }
    }
  }

  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    while (game_state->current_players != game_state->number_players) {
      // Waits for the right number of players to join.
      break;
    }

    if (game_state->current_players == game_state->number_players) {
      if (game_state->start != 1) {
        // Starts the game once the proper number of students have joined.
        game_state->start = 1;
        game_state->turn = game_state->player_list->head;
        start_game(game_state);
      }
      play_game(game_state);

      return -1;
    }
  }
  return 0;
}

void start_game(game_state* state) {
  player* current = state->player_list->head;
  for (size_t i = 0; i < state->number_players; i++) {
    // Initializes the hands for each player
    make_hand(state, current);

    card* card_ = current->hand.head;
    while (card_ != NULL) {
      card_ = card_->next;
    }
    current = current->next;
  }
  state->start = 1;
  send_message(state, 0, NULL);
}
void play_game(game_state* state) {
  while (1) {
    FILE* input_file = fdopen(state->turn->sock_num, "r+");
    char* buf = NULL;
    size_t buf_size = 0;
    ssize_t line = getline(&buf, &buf_size, input_file);
    if (line < 0) {
      perror("fail");
      break;
    }
    // illegal seek - instead of erroring and exiting send EOF and close socket.

    printf("read! %s\n", buf);
    if (buf[0] == 'u') {
      if (check_uno(state) == 1) {
        send_message(state, 1, buf);
      }
    } else if (buf[0] == '\0') {
      continue;
    } else if (strstr(buf, "draw") != NULL) {
      if (check_draw(state) == 1) {
        refill_draw(state);
      }
      draw_card(state);
      change_turn(state);

      send_message(state, 0, NULL);
      continue;
    } else {
      char card_str[5];  // NOLINT(*-magic-numbers)
      process_input(buf, card_str);
      play_uno(state, card_str);
      change_turn(state);
      send_message(state, 0, NULL);
      continue;
    }
  }
}
void broadcast_message(game_state* state, const char* buf) {
  if (buf[0] == 'u') {
    if (state->turn->hand.size == 1) {
      player* current = state->player_list->head;
      char sendline[1000];  // NOLINT(*-magic-numbers)
      (void)sprintf(sendline, "Player %zu has called uno!", state->turn->number);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      write(current->sock_num, sendline, sizeof(sendline));
      current = current->next;
      while (current != state->turn) {
        char sendlinee[1000];  // NOLINT(*-magic-numbers)
        (void)sprintf(sendlinee, "Player %zu has called uno!", state->turn->number);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
        write(current->sock_num, sendlinee, sizeof(sendlinee));
        current = current->next;
      }
    } else {
      player* current = state->player_list->head;
      char sendline2[1000];  // NOLINT(*-magic-numbers)
      (void)sprintf(sendline2, "A Player has called uno before another!");  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      write(current->sock_num, sendline2, sizeof(sendline2));
      current = current->next;
      while (current != state->turn) {
        char sendlinee2[1000];  // NOLINT(*-magic-numbers)
        (void)sprintf(sendlinee2, "A Player has called uno before another!");  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
        write(current->sock_num, sendlinee2, sizeof(sendlinee2));
        current = current->next;
      }
    }
  }
}

void send_message(game_state* state, int type, const char* buf) {
  if (type == 1) {
    // If it is a broadcast message, it constructs and send it to everyone
    broadcast_message(state, buf);
  } else {
    // If it is not a broadcast then it simply collects the player info and
    // transmits it
    player* current_player = state->player_list->head;

    for (size_t i = 0; i < state->number_players; i++) {
      char sendlin[1000];  // NOLINT(*-magic-numbers)
      (void)sprintf(sendlin, "%i/%zu/%zu/", 0, current_player->number, state->turn->number);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

      char main_card[100];  // NOLINT(*-magic-numbers)
      if (state->main.head == NULL) {
      }
      if (state->main.head != NULL) {
        (void)sprintf(main_card, " %c%zu/", state->main.head->color, state->main.head->value);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      }
      printf("%s", main_card);
      // printf("FJFHDJ\n");
      strcat(sendlin, main_card);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
      char num_cards[8];  // NOLINT(*-magic-numbers)
      (void)sprintf(num_cards, "%zu/", state->number_players);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      strcat(sendlin, num_cards);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
      printf("about to enter while loop\n");
      card* current_card = current_player->hand.head;
      while (current_card != NULL) {
        char card[7];  // NOLINT(*-magic-numbers)
        (void)sprintf(card, " %c%zu,", current_card->color, current_card->value);  // NOLINT
        strcat(sendlin, card);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
        current_card = current_card->next;
      }
      char num_players[5];  // NOLINT(*-magic-numbers)
      (void)sprintf(num_players, "/%zu/", state->number_players);  // NOLINT
      strcat(sendlin, num_players);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
      player* temp = state->player_list->head;
      char hand_size[7];                            // NOLINT(*-magic-numbers)
      sprintf(hand_size, "%zu/", temp->hand.size);  // NOLINT
      strcat(sendlin, hand_size);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
      temp = temp->next;
      while (temp != state->player_list->head) {
        char hand_sizee[7];  // NOLINT(*-magic-numbers)
        (void)sprintf(hand_sizee, "%zu/", temp->hand.size);  // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
        strcat(sendlin, hand_sizee);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
        temp = temp->next;
      }
      strcat(sendlin, "\0");  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
      write(current_player->sock_num, sendlin, 1000);  // NOLINT(*-magic-numbers)
      current_player = current_player->next;
    }
  }
}

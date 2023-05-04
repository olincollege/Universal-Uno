#include "controller.h"
void process_input(char* buf, char* processed_input) {
  // char* processed_input[strlen(buf)];
  // check to make sure it's an actual card
  if(buf[1] == 'r') {
    processed_input[0] = buf[0];
    strcat(processed_input, "10");
  } else if(buf[1] == 'S') {
    processed_input[0] = buf[0];
    strcat(processed_input, "11");
  } else if(buf[1] == '2') {
    processed_input[0] = buf[0];
    strcat(processed_input, "12");
  } else if(buf[0] == 'W' && buf[1] == '4') {
    processed_input[0] = buf[2];
    strcat(processed_input, "14");
  } else if(buf[0] == 'W') {
    processed_input[0] = buf[1];
    strcat(processed_input, "13");
  } else {
    strcpy(processed_input, buf);
  }
}


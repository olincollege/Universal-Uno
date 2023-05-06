#include "controller.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void process_input(char* buf, char* processed_input) {
  if(buf[1] == 'r') {
    processed_input[0] = buf[0];
    strcat(processed_input, "10"); //NOLINT(clang-analyzer-security.insecureAPI.strcpy)
  } else if(buf[1] == 'S') {
    processed_input[0] = buf[0];
    strcat(processed_input, "11"); //NOLINT(clang-analyzer-security.insecureAPI.strcpy)
  } else if(buf[1] == '2') {
    processed_input[0] = buf[0];
    strcat(processed_input, "12"); //NOLINT(clang-analyzer-security.insecureAPI.strcpy)
  } else if(buf[0] == 'W' && buf[1] == '4') {
    processed_input[0] = buf[2];
    strcat(processed_input, "14"); //NOLINT(clang-analyzer-security.insecureAPI.strcpy) 
  } else if(buf[0] == 'W') {
    processed_input[0] = buf[1];
    strcat(processed_input, "13"); //NOLINT(clang-analyzer-security.insecureAPI.strcpy)
  } else {
    strcpy(processed_input, buf); //NOLINT(clang-analyzer-security.insecureAPI.strcpy)
  }
}


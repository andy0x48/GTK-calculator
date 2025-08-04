//
// Created by andys on 23/07/2025.
//

#ifndef LOGIC_H
#define LOGIC_H

#define MAX_INPUT_LEN 512
#define MAX_DIGIT_PER_TERM 40

void reset_logic_state(void);

void append_input(char c);
void clear_input(void);
const char *get_input(void);
const char *eval_expression(const char *expression);

#endif //LOGIC_H

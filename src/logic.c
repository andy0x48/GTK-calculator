//
// Created by andys on 23/07/2025.
//

#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char input_buffer[MAX_INPUT_LEN] = "";
static char result[128] = "";
static char err_msg[128] = "";

/*
 *	State Management:
 */

void reset_logic_state(void) {
	input_buffer[0] = '\0';
	result[0] = '\0';
	err_msg[0] = '\0';
}

void append_input(char c) {
	size_t len = strlen(input_buffer);
	if (len < MAX_INPUT_LEN - 1) {
		if (isdigit(c) || strchr("+-*/(). ", c)) {
			input_buffer[len] = c;
			input_buffer[len + 1] = '\0';
		}
	}
}

void clear_input(void) {
	input_buffer[0] = '\0';
}

const char *get_input(void) {
	return input_buffer;
}

/*
 *	Expression Parsing:
 */

static const char *expr_ptr;

// fwd declarations
static double parse_expr(void);
static double parse_term(void);
static double parse_factor(void);
static double parse_number(void);

static void skip_spaces(void) {
	while (isspace(*expr_ptr)) expr_ptr++;
}

static int match(char expected) {
	skip_spaces();
	if (*expr_ptr == expected) {
		expr_ptr++;
		return 1;
	}
	return 0;
}

static double parse_expr(void) {
	double value = parse_term();
	while (1) {
		skip_spaces();
		if (match('+')) {
			value += parse_term();
		}
		else if (match('-')) {
			value -= parse_term();
		}
		else {
			break;
		}
	}
	return value;
}

static double parse_term(void) {
	double value = parse_factor();
	while (1) {
		skip_spaces();
		if (match('*')) {
			value *= parse_factor();
		}
		else if (match('/')) {
			double divisor = parse_factor();
			if (divisor == 0) {
				snprintf(err_msg, sizeof(err_msg), "Error: Division by zero!");
				return 0;
			}
			value /= divisor;
		}
		else {
			break;
		}
	}
	return value;
}

static double parse_factor(void) {
	skip_spaces();
	if (match('(')) {
		double value = parse_expr();
		if (!match(')')) {
			snprintf(err_msg, sizeof(err_msg), "Error: Missing ')'");
			return 0;
		}
		return value;
	}
	return parse_number();
}

static double parse_number(void) {
	skip_spaces();

	char num_buffer[64] = "";
	int i = 0;

	if (*expr_ptr == '-' || *expr_ptr == '+') {
		num_buffer[i++] = *expr_ptr++;
	}

	while (isdigit(*expr_ptr) || *expr_ptr == '.') {
		if (i < (int)sizeof(num_buffer) - 1) {
			num_buffer[i++] = *expr_ptr++;
		}
		else {
			break;
		}
	}
	num_buffer[i] = '\0';

	if (i == 0 || (num_buffer[0] == '-' && num_buffer[1] == '\0')) {
		snprintf(err_msg, sizeof(err_msg), "Error: Invalid number!");
		return 0;
	}
	return strtod(num_buffer, NULL);
}

const char *eval_expression(const char *expression) {
	err_msg[0] = '\0';

	expr_ptr = expression;
	skip_spaces();

	double result_val = parse_expr();

	skip_spaces();
	if (*expr_ptr != '\0') {
		snprintf(err_msg, sizeof(err_msg), "Error: Unexpected character '%c'", *expr_ptr);
		return err_msg;
	}

	snprintf(result, sizeof(result), "%.10f", result_val);
	return result;
}
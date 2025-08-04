//
// Created by andys on 23/07/2025.
//

#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

static char input_buffer[MAX_INPUT_LEN] = "";
static char result[128] = "";
static char err_msg[128] = "";
static int err_flag = 0;

/*
 *	State Management:
 */

void reset_logic_state(void) {
	input_buffer[0] = '\0';
	result[0] = '\0';
	err_msg[0] = '\0';
	err_flag = 0;
}

void append_input(char c) {
	size_t len = strlen(input_buffer);
	if (len < MAX_INPUT_LEN - 1) {
		// Remove space from allowed characters to avoid parsing issues
		if (isdigit(c) || strchr("+-*/()", c) || c == '.') {
			input_buffer[len] = c;
			input_buffer[len + 1] = '\0';
		}
	}
}

void clear_input(void) {
	input_buffer[0] = '\0';
	err_flag = 0;
}

const char *get_input(void) {
	return input_buffer;
}

// Check for last eval error
int has_eval_error(void) {
	return err_flag;
}

// Get the last error msg
const char *eval_error_msg(void) {
	return err_msg;
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
	if (err_flag) return 0;

	double value = parse_term();
	while (!err_flag) {
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
	if (err_flag) return 0;

	double value = parse_factor();
	while (!err_flag) {
		skip_spaces();
		if (match('*')) {
			value *= parse_factor();
		}
		else if (match('/')) {
			double divisor = parse_factor();
			if (!err_flag && divisor == 0) {
				snprintf(err_msg, sizeof(err_msg), "Error: Division by zero!");
				err_flag = 1;
				return 0;
			}
			if (!err_flag) {
				value /= divisor;
			}
		}
		else {
			break;
		}
	}
	return value;
}

static double parse_factor(void) {
	if (err_flag) return 0;

	skip_spaces();
	if (match('(')) {
		double value = parse_expr();
		if (!err_flag && !match(')')) {
			snprintf(err_msg, sizeof(err_msg), "Error: Missing ')'");
			err_flag = 1;
			return 0;
		}
		return value;
	}
	return parse_number();
}

static double parse_number(void) {
	if (err_flag) return 0;

	skip_spaces();

	char num_buffer[64] = "";
	int i = 0;
	int has_decimal = 0;

	if (*expr_ptr == '-' || *expr_ptr == '+') {
		num_buffer[i++] = *expr_ptr++;
	}

	while (isdigit(*expr_ptr) || (*expr_ptr == '.' && !has_decimal)) {
		if (*expr_ptr == '.') {
			has_decimal = 1;
		}
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
		err_flag = 1;
		return 0;
	}
	return strtod(num_buffer, NULL);
}

// Result formatting helper function
static void format_result(double value, char *buffer, size_t buffer_size) {
	// Decimal or integer check
	if (fabs(value - round(value)) < 1e-10) {
		snprintf(buffer, buffer_size, "%.0f", value);	// integer
	}
	else {
		snprintf(buffer, buffer_size, "%.12f", value);	// decimal max

		// Remove trailing 0's
		int len = strlen(buffer);
		while (len > 1 && buffer[len - 1] == '0' && buffer[len - 2] != '.') {
			buffer[--len] = '\0';
		}

		// Remove decimal point if last char
		if (len > 1 && buffer[len - 1] == '.') {
			buffer[--len] = '\0';
		}
	}
}

const char *eval_expression(const char *expression) {
	// Reset error states
	err_msg[0] = '\0';
	err_flag = 0;

	// Check empty expression
	if (!expression || strlen(expression) == 0) {
		snprintf(err_msg, sizeof(err_msg), "Error: Empty expression!");
		err_flag = 1;
		return err_msg;
	}

	expr_ptr = expression;
	skip_spaces();

	// Check empty expression AFTER space skipping
	if (*expr_ptr == '\0') {
		snprintf(err_msg, sizeof(err_msg), "Error: Empty expression!");
		err_flag = 1;
		return err_msg;
	}

	double result_val = parse_expr();

	// Check remaining chars if no errors so far
	if (!err_flag) {
		skip_spaces();
		if (*expr_ptr != '\0') {
			snprintf(err_msg, sizeof(err_msg), "Error: Unexpected character '%c'", *expr_ptr);
			err_flag = 1;
			return err_msg;
		}
	}

	if (err_flag) {
		return err_msg;
	}

	format_result(result_val, result, sizeof(result));
	//snprintf(result, sizeof(result), "%.10f", result_val);
	return result;
}
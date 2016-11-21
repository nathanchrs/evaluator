#include <string.h>
#include "evaluator.h"

#define DECIMAL_POINT '.'

#define ALLOW_UNARY_POSITIVE_OPERATOR
#define ALLOW_CONSECUTIVE_UNARY_OPERATORS

EvalResult expression(const char *input, size_t *pos, size_t end);

EvalResult digit(const char *input, size_t *pos, size_t end) {
	if (*pos <= end && '0' <= input[*pos] && input[*pos] <= '9') {
		return integralResult(input[(*pos)++] - '0');
	} else {
		return syntaxErrorResult(*pos);
	}
}

EvalResult fractional(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = digit(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult res2 = fractional(input, pos, end);
	if (isError(res2)) {
		return fractionalResult(0.1 * (double) res1.integralValue); // digit
	}

	return fractionalResult(0.1 * ((double) res1.integralValue + res2.fractionalValue)); // digit_fractional
}

EvalResult integer(const char *input, size_t *pos, size_t end) {
	size_t initialPos = *pos;

	EvalResult res1 = digit(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult res2 = integer(input, pos, end);
	if (isError(res2)) {
		return integralResult(res1.integralValue); // digit
	}

	long long exp = 1;
	int i;
	for (i = 0; i < (*pos)-initialPos-1; i++) exp *= 10;

	return integralResult(res1.integralValue * exp + res2.integralValue); // digit_integer
}

EvalResult number(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = integer(input, pos, end);
	if (isError(res1)) return res1;

	if (*pos <= end && input[*pos] == DECIMAL_POINT) {
		(*pos)++;
		EvalResult res2 = fractional(input, pos, end);
		if (isError(res2)) return res2;
		return fractionalResult((double) res1.integralValue + res2.fractionalValue); // integer.fractional
	}
	
	return integralResult(res1.integralValue); // integer
}

EvalResult factorWithoutUnary(const char *input, size_t *pos, size_t end) {
	if (*pos <= end && input[*pos] == '(') {
		(*pos)++;
		EvalResult res = expression(input, pos, end);
		if (*pos <= end && input[*pos] == ')') {
			(*pos)++;
			return res; // (expression)
		} else {
			return syntaxErrorResult(*pos); // missing ending parentheses
		}
	} else {
		return number(input, pos, end); // number
	}
}

EvalResult factor(const char *input, size_t *pos, size_t end) {
	if (*pos <= end && input[*pos] == '-') { // unary negative operator
		(*pos)++;
		return multiply(factor(input, pos, end), integralResult(-1));
	}

#ifdef ALLOW_UNARY_POSITIVE_OPERATOR
	else if (*pos <= end && input[*pos] == '+') { // unary positive operator
		(*pos)++;
		return factor(input, pos, end);
	}
#endif

	return factorWithoutUnary(input, pos, end);
}

// term -> term*factor | term/factor | factor
// Modified to factor_((*|/)_factor)* iteration to prevent left-recursion and keep left-associaticity
EvalResult term(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = factor(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult resAccumulator = res1;
	while(1) {
		if (*pos <= end && input[*pos] == '*') {
			(*pos)++;

			EvalResult res2 = factor(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = multiply(resAccumulator, res2);

		} else if (*pos <= end && input[*pos] == '/') {
			(*pos)++;

			EvalResult res2 = factor(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = divide(resAccumulator, res2);

		} else {
			break;
		}
	}
	return resAccumulator;
}

// expression -> expression+term | expression-term | term
// Modified to term_((+|-)_term)*  iteration to prevent left-recursion and keep left-associaticity
EvalResult expression(const char *input, size_t *pos, size_t end) {
	EvalResult res1 = term(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult resAccumulator = res1;
	while(1) {
		if (*pos <= end && input[*pos] == '+') {
			(*pos)++;

			EvalResult res2 = term(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = add(resAccumulator, res2);

		} else if (*pos <= end && input[*pos] == '-') {
			(*pos)++;

			EvalResult res2 = term(input, pos, end);
			if (isError(res2)) resAccumulator = res2;
			resAccumulator = subtract(resAccumulator, res2);

		} else {
			break;
		}
	}
	return resAccumulator;
}

EvalResult evaluate(const char *input) {
	size_t pos = 0;
	size_t length = strlen(input);

	#ifndef ALLOW_CONSECUTIVE_UNARY_OPERATORS
		if (length > 1) {
			size_t i;
			for (i = 1; i < length; i++) {
				if ((input[i] == '+' || input[i] == '-') && (input[i-1] == '+' || input[i-1] == '-')) {
					return syntaxErrorResult(i);
				}
			}
		}
	#endif

	EvalResult res = expression(input, &pos, length);
	if (pos != length) {
		return syntaxErrorResult(pos); // there are still unconsumed input characters remaining
	}
	return res;
}

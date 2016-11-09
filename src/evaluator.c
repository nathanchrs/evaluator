
#include <string.h>
#include "evaluator.h"

#define DECIMAL_POINT '.'

EvalResult expression(const char *input, size_t *pos, const size_t end);

EvalResult digit(const char *input, size_t *pos, const size_t end) {
	if (*pos <= end && '0' <= input[*pos] && input[*pos] <= '9') {
		return integralResult(input[(*pos)++] - '0');
	} else {
		return syntaxErrorResult(*pos);
	}
}

EvalResult fractional(const char *input, size_t *pos, const size_t end) {
	EvalResult res1 = digit(input, pos, end);
	if (isError(res1)) return res1;

	EvalResult res2 = fractional(input, pos, end);
	if (isError(res2)) {
		return fractionalResult(0.1 * (double) res1.integralValue); // digit
	}

	return fractionalResult(0.1 * ((double) res1.integralValue + res2.fractionalValue)); // digit_fractional
}

EvalResult integer(const char *input, size_t *pos, const size_t end) {
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

EvalResult number(const char *input, size_t *pos, const size_t end) {
	char sign = 1;
	if (*pos <= end && input[*pos] == '-') {
		sign = -1;
		(*pos)++;
	}

	EvalResult res1 = integer(input, pos, end);
	if (isError(res1)) return res1;

	if (*pos <= end && input[*pos] == DECIMAL_POINT) {
		(*pos)++;
		EvalResult res2 = fractional(input, pos, end);
		if (isError(res2)) return res2;
		return fractionalResult(((double) res1.integralValue + res2.fractionalValue) * (double) sign); // integer.fractional

	} else {
		return integralResult(res1.integralValue * (long long) sign); // integer
	}
}

EvalResult factor(const char *input, size_t *pos, const size_t end) {
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

EvalResult term(const char *input, size_t *pos, const size_t end) {
	EvalResult res1 = factor(input, pos, end);
	if (isError(res1)) return res1;

	if (*pos <= end && input[*pos] == '*') { // factor*term
		(*pos)++;

		EvalResult res2 = term(input, pos, end);
		if (isError(res2)) return res2;
		return multiply(res1, res2);

	} else if (*pos <= end && input[*pos] == '/') { // factor/term
		(*pos)++;

		EvalResult res2 = term(input, pos, end);
		if (isError(res2)) return res2;
		return divide(res1, res2);

	} else {
		return res1; // factor
	}
}

EvalResult expression(const char *input, size_t *pos, const size_t end) {
	EvalResult res1 = term(input, pos, end);
	if (isError(res1)) return res1;

	if (*pos <= end && input[*pos] == '+') { // term+expression
		(*pos)++;

		EvalResult res2 = expression(input, pos, end);
		if (isError(res2)) return res2;
		return add(res1, res2);

	} else if (*pos <= end && input[*pos] == '-') { // term-expression
		(*pos)++;

		EvalResult res2 = expression(input, pos, end);
		if (isError(res2)) return res2;
		return subtract(res1, res2);

	} else {
		return res1; // term
	}
}

EvalResult evaluate(const char *input) {
	size_t pos = 0;
	size_t length = strlen(input);
	EvalResult res = expression(input, &pos, length);
	if (pos != length) {
		return syntaxErrorResult(pos); // there are still unconsumed input characters remaining
	}
	return res;
}
